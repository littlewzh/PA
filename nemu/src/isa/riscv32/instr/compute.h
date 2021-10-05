def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}
def_EHelper(li) {
  rtl_li(s,ddest,id_src1->imm);
}
def_EHelper(auipc) {
  rtl_li(s,s0,id_src1->imm);
  rtl_addi(s,ddest,s0,s->pc);
}
