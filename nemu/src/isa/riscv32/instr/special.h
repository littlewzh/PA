def_EHelper(inv) {
  rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(nemu_trap) {
  rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}
def_EHelper(ecall){
   
}
def_EHelper(csrrs){
   *ddest=*dsrc2;
   *dsrc2=*dsrc2|*dsrc1;
}
def_EHelper(csrrc){
   *ddest=*dsrc2;
   *dsrc2=*dsrc2&*dsrc1;
}
def_EHelper(csrrw){
   *ddest=*dsrc2;
   *dsrc2=*dsrc1;
}