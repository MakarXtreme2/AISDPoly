grammar polynom;
NEWLINE: '\r'? '\n' -> skip;
WS: [ \t\r\n]+ -> skip;
MUL: '*';
DIV: '/';
ADD: '+';
SUB: '-';
poly: poly op=(MUL | DIV) poly  # MulDiv
    | poly op=(ADD | SUB) poly  # AddSub
    | mon                       # Monom
    | '(' poly ')'              # Parens
    ;
mon: INT? ((X | Y | Z | W)+ARROWUP?)+
    | INT
    ;
X: 'x';
Y: 'y';
Z: 'z';
W: 'w';
ARROWUP: '^'INT;
INT: [0-9]+;
