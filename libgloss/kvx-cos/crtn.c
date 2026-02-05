asm(    "       .section .init     \n"
        "       lw $r0 = 16[$r12]  \n"
        "       add $r12 = $r12, 32\n"
        "       ;;                 \n"
        "       set $ra = $r0      \n"
        "       ;;                 \n"
        "       ret                \n"
        "       ;;                 \n"
        "       .previous          \n");

asm(    "       .section .fini     \n"
        "       lw $r0 = 16[$r12]  \n"
        "       add $r12 = $r12, 32\n"
        "       ;;                 \n"
        "       set $ra = $r0      \n"
        "       ;;                 \n"
        "       ret                \n"
        "       ;;                 \n"
        "       .previous          \n");
