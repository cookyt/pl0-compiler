int x,y,z,v,w;
    procedure a;
    int x,y,u,v;
        procedure b;
        int y,z,v;
            procedure c;
            int y,z;
            begin
                z:=1;
                x:=y+z+w
            end;
        begin
            y:=x+u+w;
            call c
        end;
    begin
        z:=2;
        u:=z+w;
        call b
    end;

begin
    x:=1; y:=2; z:=3; v:=4; w:=5;
    x:=v+w;
    out z;
    call a;
end.
