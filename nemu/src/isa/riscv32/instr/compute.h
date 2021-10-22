def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}
def_EHelper(addi) {
  //rtl_li(s,ddest,id_src2->imm);
  //rtl_sext(s,ddest,ddest,12);
  rtl_addi(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(auipc) {
  rtl_li(s,t0,id_src1->imm);
  rtl_addi(s,ddest,t0,s->pc);
}
def_EHelper(add) {
  rtl_add(s,ddest,dsrc1,dsrc2);
}
def_EHelper(sub) {
  //rtl_not(s,dsrc2,dsrc2);
  //rtl_addi(s,dsrc2,dsrc2,1);
  rtl_sub(s,ddest,dsrc1,dsrc2);
}
def_EHelper(sll) {
  rtl_sll(s,ddest,dsrc1,dsrc2);
}
def_EHelper(slt) {
  //if(rtl_setrelop(s,4,dsrc1,dsrc2)) {
  //  rtl_li(s,ddest,1);
  //}
  //else {rtl_li(s,ddest,0);}
  rtl_setrelop(s,4,ddest,dsrc1,dsrc2);
}
def_EHelper(srl){
   rtl_srl(s,ddest,dsrc1,dsrc2);
}
def_EHelper(sra){
   rtl_sra(s,ddest,dsrc1,dsrc2);
}
def_EHelper(slti) {
  rtl_setrelopi(s,4,ddest,dsrc1,id_src2->imm);
}
def_EHelper(andi){
  rtl_andi(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(sltu) {
  //if(rtl_setrelop(s,8,dsrc1,dsrc2)) {
  //  rtl_li(s,ddest,1);
  //}
  //else {rtl_li(s,ddest,0);}
  rtl_setrelop(s,8,ddest,dsrc1,dsrc2);
}                                        //fuhaowenti
def_EHelper(sltiu) {
  //rtl_li(s,ddest,id_src2->imm);
  //rtl_sext(s,ddest,ddest,12);
  //if(*dsrc1<id_src2->imm) {
   // rtl_li(s,ddest,1);
  //}
  //else {rtl_li(s,ddest,0);}
  rtl_setrelopi(s,8,ddest,dsrc1,id_src2->imm);
}
def_EHelper(xor) {
  *ddest=*dsrc1^*dsrc2;
}
def_EHelper(or) {
  *ddest=*dsrc1|*dsrc2;
}
def_EHelper(and){
  rtl_and(s,ddest,dsrc1,dsrc2);
}
def_EHelper(srai) {
   rtl_srai(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(xori) {
  rtl_xori(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(mul){
  rtl_mulu_lo(s,ddest,dsrc1,dsrc2);
}
def_EHelper(div){
  rtl_divs_q(s,ddest,dsrc1,dsrc2);
}
def_EHelper(ori){
  rtl_ori(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(slli){
  rtl_slli(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(srli){
  rtl_srli(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(rem){
  rtl_divs_r(s,ddest,dsrc1,dsrc2);
}
def_EHelper(mulh){
  rtl_muls_hi(s,ddest,dsrc1,dsrc2);
}
def_EHelper(divu){
   rtl_divu_q(s,ddest,dsrc1,dsrc2);
}
def_EHelper(remu){
  rtl_divu_r(s,ddest,dsrc1,dsrc2);
}