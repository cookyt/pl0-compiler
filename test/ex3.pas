int f, n;
    procedure fact;
    int ans1;
    begin
        ans1:=n;
        n:= n-1;
        if n <= 0 then f := 1;
        if n >  0 then call fact;
        f:=f*ans1;
    end;
begin
    read n;
    call fact;
    write f;
end.
