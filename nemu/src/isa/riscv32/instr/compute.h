def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}
def_EHelper(addi) {
  rtl_addi(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(auipc) {
  rtl_li(s,s0,id_src1->imm);
  rtl_addi(s,ddest,s0,s->pc);
}
def_EHelper(add) {
  rtl_add(s,ddest,dsrc1,dsrc2);
}
def_EHelper(sub) {
  rtl_not(s,dsrc2,dsrc2);
  rtl_addi(s,dsrc2,dsrc2,1);
  rtl_add(s,ddest,dsrc1,dsrc2);
}
