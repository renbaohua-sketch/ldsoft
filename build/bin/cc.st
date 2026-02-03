(* 梯形图生成的ST代码 *)
PROGRAM LadderProgram

VAR
    Start : BOOL;
    Stop : BOOL;
    Motor : BOOL;
END_VAR

(* 主程序 *)
Motor := Start AND NOT Stop;

END_PROGRAM
