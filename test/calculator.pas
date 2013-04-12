/* four function calculator */
const mul=1, div=2, add=3, sub=4;
int op, value, in1, in2;
begin
    op := 1;
    while op <> 0 do
    begin
        read op;
        if op <> 0 then
        begin
            read in1;
            read in2;
            if op = mul then
                value := in1*in2
            else if op = div then
                value := in1/in2
            else if op = add then
                value := in1+in2
            else if op = sub then
                value := in1-in2
            else
                op := 0;
            
            write value;
        end;
    end;
end.
