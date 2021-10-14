#include "../local-include/reg.h"
#include <cpu/ifetch.h>
#include <isa-all-instr.h>

def_all_THelper();

static uint32_t get_instr(Decode *s) {
  return s->isa.instr.val;
}

// decode operand helper
#define def_DopHelper(name) \
  void concat(decode_op_, name) (Decode *s, Operand *op, word_t val, bool flag)

static def_DopHelper(i) {
  op->imm = val;
}

static def_DopHelper(r) {                                //指令译码，用于分出不同的操作数
  bool is_write = flag;
  static word_t zero_null = 0;
  op->preg = (is_write && val == 0) ? &zero_null : &gpr(val);
}
static def_DHelper(R) {
  decode_op_r(s, id_dest, s->isa.instr.r.rd, true);
  decode_op_r(s, id_src1, s->isa.instr.r.rs1, false);
  decode_op_r(s, id_src2, s->isa.instr.r.rs2, false);

}
static def_DHelper(I) {
  decode_op_r(s, id_src1, s->isa.instr.i.rs1, false);
  decode_op_i(s, id_src2, s->isa.instr.i.simm11_0, false);
  decode_op_r(s, id_dest, s->isa.instr.i.rd, true);
}

static def_DHelper(U) {
  decode_op_i(s, id_src1, s->isa.instr.u.imm31_12 << 12, true);
  decode_op_r(s, id_dest, s->isa.instr.u.rd, true);
}

static def_DHelper(S) {
  decode_op_r(s, id_src1, s->isa.instr.s.rs1, false);
  sword_t simm = (s->isa.instr.s.simm11_5 << 5) | s->isa.instr.s.imm4_0;
  decode_op_i(s, id_src2, simm, false);
  decode_op_r(s, id_dest, s->isa.instr.s.rs2, false);
}
static def_DHelper(J) {
  decode_op_r(s, id_dest, s->isa.instr.j.rd,true);
  sword_t simm= (((s->isa.instr.j.simm20<<20)|s->isa.instr.j.imm19_12<<12)|s->isa.instr.j.imm11<<11)|s->isa.instr.j.imm10_1<<1;
  decode_op_i(s, id_src1, simm, false);
}
static def_DHelper(B) {
  sword_t simm= (((s->isa.instr.b.imm11<<11)|s->isa.instr.b.imm4_1<<1)|s->isa.instr.b.imm10_5<<5)|s->isa.instr.b.simm12<<12;
  decode_op_i(s, id_dest,simm,false);
  decode_op_r(s, id_src1,s->isa.instr.b.rs1,false);
  decode_op_r(s, id_src2,s->isa.instr.b.rs2,false);                      //what is true or false used for? 
}
def_THelper(load) {                                                    //找到标识该指令唯一ID
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", lw);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", lbu);
  return EXEC_ID_inv;
}

def_THelper(store) {
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", sw);
  return EXEC_ID_inv;
}
def_THelper(branch) {
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", beq);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", bne);
  def_INSTR_TAB("??????? ????? ????? 101 ????? ????? ??", bge);
  return EXEC_ID_inv;
}
def_THelper(inter) {
  def_INSTR_TAB("0000000 ????? ????? 000 ????? ????? ??",add);
  def_INSTR_TAB("0100000 ????? ????? 000 ????? ????? ??",sub);
  def_INSTR_TAB("??????? ????? ????? 011 ????? ????? ??",sltu);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??",xor);
  def_INSTR_TAB("??????? ????? ????? 110 ????? ????? ??",xor);
  return EXEC_ID_inv;
}
def_THelper(interi) {
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??",addi);
  def_INSTR_TAB("??????? ????? ????? 011 ????? ????? ??",sltiu);
  def_INSTR_TAB("0100000 ????? ????? 101 ????? ????? ??",srai);
  return EXEC_ID_inv;
}
def_THelper(main) {
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00000 11", I     , load);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01000 11", S     , store);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01101 11", U     , lui);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00100 11", I     , interi);   //this alse exits some problems
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00101 11", U     , auipc);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11011 11", J     , jal);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11000 11", B     , branch);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01100 11", R     , inter);
  def_INSTR_TAB("??????? ????? ????? ??? ????? 11001 11"     , ret);//////////???????
  def_INSTR_TAB  ("??????? ????? ????? ??? ????? 11010 11",         nemu_trap);
  return table_inv(s);
};

int isa_fetch_decode(Decode *s) {                                //取指令，并找到该指令对应的操作类型
  s->isa.instr.val = instr_fetch(&s->snpc, 4);
  int idx = table_main(s);
  return idx;
}
