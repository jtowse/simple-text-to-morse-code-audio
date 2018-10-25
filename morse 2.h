//#include<string.h>
char *morse(char test, char mors[]);

char *morse(char test, char mors[]){
    char number[100];
    //char done[100];
    switch(test){
        case 'A': case 'a':
            strcpy(number,  "91029");
            break;
        case 'B': case 'b':
            strcpy(number, "920101019");
            break;    
        case 'C': case 'c':
            strcpy(number, "920102019");
            break;
        case 'D': case 'd':
            strcpy(number, "9201019");
            break;
        case 'E': case 'e':
            strcpy(number, "919");
            break;
        case 'F': case 'f':
            strcpy(number, "910102019");
            break;    
        case 'G': case 'g':
            strcpy(number, "9202019");
            break;
        case 'H': case 'h':
            strcpy(number, "910101019");
            break;   
        case 'I': case 'i':
            strcpy(number, "91019");
            break;
        case 'J': case 'j':
            strcpy(number, "910202029");
            break;    
        case 'K': case 'k':
            strcpy(number, "9201029");
            break;
        case 'L': case 'l':
            strcpy(number, "910201019");
            break;
        case 'M': case 'm':
            strcpy(number, "92029");
            break;
        case 'N': case 'n':
            strcpy(number, "92019");
            break;    
        case 'O': case 'o':
            strcpy(number, "9202029");
            break;
        case 'P': case 'p':
            strcpy(number, "910202019");
            break; 
        case 'Q': case 'q':
            strcpy(number, "920201029");
            break;
        case 'R': case 'r':
            strcpy(number, "9102019");
            break;    
        case 'S': case 's':
            strcpy(number, "9101019");
            break;
        case 'T': case 't':
            strcpy(number, "929");
            break;
        case 'U': case 'u':
            strcpy(number, "9101029");
            break;
        case 'V': case 'v':
            strcpy(number, "910101029");
            break;    
        case 'W': case 'w':
            strcpy(number, "9102029");
            break;
        case 'X': case 'x':
            strcpy(number, "9102029");
            break;   
        case 'Y': case 'y':
            strcpy(number, "920101029");
            break;
        case 'Z': case 'z':
            strcpy(number, "920201019");
            break;    
        case '1':
            strcpy(number, "91020202029");
            break;
         case '2':
            strcpy(number, "91010202029");
            break;           
        case '3':
            strcpy(number, "91010102029");
            break;
        case '4':
            strcpy(number, "91010101029");
            break;
        case '5':
            strcpy(number, "91010101019");
            break;
        case '6':
            strcpy(number, "92010101019");
            break;
        case '7':
            strcpy(number, "92020101019");
            break;
        case '8':
            strcpy(number, "92020201019");
            break;      
        case '9':
            strcpy(number, "92020202019");
            break;
        case '0':
            strcpy(number, "92020202029");
            break;
        default:
            strcpy(number, "0");                                    
    }

    strcpy(mors, number);
    //printf("string is %s", done);

return mors;
}