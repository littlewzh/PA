def_EHelper(inv) {
  rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(nemu_trap) {
  rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}
def_EHelper(ecall){
   s->dnpc=isa_raise_intr(gpr(17), s->pc);
   //rtl_addi(s,&gpr(1),rz,(s->pc)+4);
   printf("%08x\n",s->dnpc);
}
def_EHelper(csrrs){
   *ddest=*dsrc2;
   *dsrc2=(*dsrc2)|(*dsrc1);
   //printf("%08x\n",*dsrc2);
}
def_EHelper(csrrc){
   *ddest=*dsrc2;
   *dsrc2=(*dsrc2)&(*dsrc1); 
   //printf("%08x\n",*dsrc2);
}
def_EHelper(csrrw){
   *ddest=*dsrc2;
   *dsrc2=*dsrc1;
   //printf("%08x\n",*dsrc2);
}
def_EHelper(mret){
  s->dnpc=cpu.mepc+4;
}