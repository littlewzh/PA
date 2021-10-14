def_EHelper(beq){
    rtl_setrelop(s,2,t0,dsrc1,dsrc2);
    if(*t0){
         //rtl_li(s,t0,id_dest->imm);
        //rtl_sext(s,t0,t0,12);
        s->dnpc=(s->pc)+(id_dest->imm);
    }
}
def_EHelper(bne){
    rtl_setrelop(s,3,t0,dsrc1,dsrc2);
    if(*t0){
        //rtl_li(s,t0,id_dest->imm);
        //rtl_sext(s,t0,t0,12);
        s->dnpc=(s->pc)+(id_dest->imm);
    }
}
def_EHelper(bge) {
    rtl_setrelop(s,5,t0,dsrc1,dsrc2);
    if(*t0){
        //rtl_li(s,t0,id_dest->imm);
        //rtl_sext(s,t0,t0,12);
        s->dnpc=(s->pc)+(id_dest->imm);
    }
}
def_EHelper(blt) {
    rtl_setrelop(s,4,t0,dsrc1,dsrc2);
    if(*t0){
        //rtl_li(s,t0,id_dest->imm);
        //rtl_sext(s,t0,t0,12);
        s->dnpc=(s->pc)+(id_dest->imm);
    }
}
def_EHelper(bltu) {
    rtl_setrelop(s,8,t0,dsrc1,dsrc2);
    if(*t0){
        //rtl_li(s,t0,id_dest->imm);
        //rtl_sext(s,t0,t0,12);
        s->dnpc=(s->pc)+(id_dest->imm);
    }
}
def_EHelper(bgeu) {
    rtl_setrelop(s,9,t0,dsrc1,dsrc2);
    if(*t0){
        //rtl_li(s,t0,id_dest->imm);
        //rtl_sext(s,t0,t0,12);
        s->dnpc=(s->pc)+(id_dest->imm);
    }
}