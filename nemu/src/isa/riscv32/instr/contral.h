def_EHelper(jal) {
  rtl_li(s,s0,4);
  rtl_addi(s,ddest,s0,s->pc);
  rtl_li(s,s1,id_src1->imm);
  rtl_sext(s,s2,s1,4);
  s->dnpc=(s->snpc)+(*s2);
}