def_EHelper(jal) {
  //rtl_li(s,ddest,id_src1->imm);
  //rtl_sext(s,ddest,ddest,20);
  s->dnpc=(s->pc)+id_src1->imm;
  rtl_addi(s,ddest,rz,(s->pc)+4);
}
def_EHelper(jalr){
  
  s->dnpc=(*dsrc1+id_src2->imm)&0xfffffffe;
  rtl_addi(s,ddest,rz,(s->pc)+4);
}
def_EHelper(ret) {
  s->dnpc=cpu.gpr[1]._32;
}