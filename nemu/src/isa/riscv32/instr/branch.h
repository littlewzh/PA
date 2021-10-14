def_EHelper(beq){
    if(*dsrc1==*dsrc2){
         //rtl_li(s,t0,id_dest->imm);
        //rtl_sext(s,t0,t0,12);
        s->dnpc=(s->pc)+(id_dest->imm);
    }
}
def_EHelper(bne){
    if(*dsrc1!=*dsrc2){
        //rtl_li(s,t0,id_dest->imm);
        //rtl_sext(s,t0,t0,12);
        s->dnpc=(s->pc)+(id_dest->imm);
    }
}
def_EHelper(bge) {
    int a,b;
    a=*dsrc1;
    b=*dsrc2;
    if(a>=b){
        //rtl_li(s,t0,id_dest->imm);
        //rtl_sext(s,t0,t0,12);
        s->dnpc=(s->pc)+(id_dest->imm);
    }
}