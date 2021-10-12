def_EHelper(jal) {
  rtl_li(s,s0,4);
  rtl_addi(s,ddest,s0,s->pc);
  rtl_li(s,s1,id_src1->imm);
  rtl_sext(s,s2,s1,20);
  s->dnpc=(s->pc)+(*s2);
}
def_EHelper(ret) {
  s->dnpc=cpu.gpr[1]._32;
}