program wconvert;

uses crt, SysUtils;

Const//el mundo de karel es una cuadricula, cada cuadro tiene paredes dependiendo de los bits que tenga encendidos, estos son los bits de cada pared
    PNORTE = 1;
    PESTE  = 2;
    PSUR   = 4;
    POESTE = 8;
type
    TTokenMundo = record
        P1, P2, P3 : WORD;
    end;
    Casilla = record
        pared, zumbadores : integer;
    end;
var
   f:file of TTokenMundo;
   TokenMundo:TTokenMundo;
   _archivoEntrada:string;
   alto, ancho, mochila : integer;
   columna, fila,orientacion:integer;
   s_orientacion, s_paredes:string;
   NumParedes, NumZumb : integer;
   mundo: array[1..100,1..100] of Casilla;
   i,j,contador,c: integer;
   k:real;

begin
    if ParamCount < 1 then begin
        writeln('Convertir mundos de .MDO a .json: wconvert mundo1.MDO [mundo2.mdo, [...]');
        Halt;
    end;
    _archivoEntrada:=ParamStr(1);

    assign(f,_archivoEntrada);
    reset(f);
    read(f,TokenMundo);

    if (TokenMundo.P1 <> 16715) or (TokenMundo.P2 <> 17746) or (TokenMundo.P3 <> 8268) then begin
        writeln('El formato de archivo del mundo de entrada es incorrecto!');
        Close(f);
        Halt;
    end;

    read(f,TokenMundo);
    if (TokenMundo.P1 <> 19791) or (TokenMundo.P2 <> 11849) then begin
        writeln('El formato de archivo del mundo de entrada es incorrecto!');
        Close(f);
        Halt;
    end;

    read(f,TokenMundo);
    ancho:=TokenMundo.P1;
    alto:=TokenMundo.P2;
    mochila:=TokenMundo.P3;

    Read(f,TokenMundo);
    columna:=TokenMundo.P1;
    fila:=TokenMundo.P2;
    orientacion:=TokenMundo.P3;
    if orientacion = 1 then begin
        s_orientacion := 'norte';
    end;
    if orientacion = 2 then begin
        s_orientacion := 'este';
    end;
    if orientacion = 3 then begin
        s_orientacion := 'sur';
    end;
    if orientacion = 4 then begin
        s_orientacion := 'oeste';
    end;

    writeln('{');
    writeln('    "karel": {');
    writeln('        "posicion": [');
    writeln('            ', fila, ',');
    writeln('            ', columna);
    writeln('        ],');
    writeln('        "orientacion": "',s_orientacion,'",');
    if mochila = -1 then begin
        writeln('        "mochila": "inf"');
    end else begin
        writeln('        "mochila": ',mochila);
    end;
    writeln('    },');
    writeln('    "casillas": [');
    //las casillas
    Read(f,TokenMundo);
    NumParedes:=TokenMundo.P1;
    NumZumb:=TokenMundo.P2;

    for i:=1 to 100 do begin
        for j:=1 to 100 do begin
            mundo[i,j].pared := 0;
            mundo[i,j].zumbadores := 0;
        end;
    end;
    for i:=1 to NumParedes do begin
        Read(f,TokenMundo);
        if (TokenMundo.P2 = 100) then begin
            TokenMundo.P3 := TokenMundo.P3-1;
        end;
        if (TokenMundo.P2 = 1) then begin
            TokenMundo.P3 := TokenMundo.P3-4;
        end;
        if (TokenMundo.P1 = 100) then begin
            TokenMundo.P3 := TokenMundo.P3-2;
        end;
        if (TokenMundo.P1 = 1) then begin
            TokenMundo.P3 := TokenMundo.P3-8;
        end;
        if TokenMundo.P3>0 then begin
            mundo[TokenMundo.P2,TokenMundo.P1].pared := TokenMundo.P3;
        end;
    end;
    for i:=1 to NumZumb do begin
        Read(f,TokenMundo);
        mundo[TokenMundo.P2,TokenMundo.P1].zumbadores := TokenMundo.P3;
    end;
    contador:=0;//contador del numero de casillas
    for i:=1 to 100 do begin
        for j:=1 to 100 do begin
            if (mundo[i,j].pared > 0) or (mundo[i,j].zumbadores <> 0) then begin
                if contador>0 then begin
                    writeln('        ,{');
                    contador := 0;
                end else begin
                    writeln('        {');
                end;
                if mundo[i,j].zumbadores = -1 then begin
                    writeln('            "zumbadores": "inf",');
                end else begin
                    writeln('            "zumbadores": ',mundo[i,j].zumbadores,',');
                end;
                s_paredes:='';
                c:=0;
                k:=8;
                while k>=1 do begin
                    if mundo[i,j].pared >= k then begin
                        if c>0 then begin
                            s_paredes := s_paredes+',';
                            c:=0;
                        end;
                        if k = 8 then begin
                            s_paredes := s_paredes+'"oeste"';
                            mundo[i,j].pared := mundo[i,j].pared-8;
                        end;
                        if k = 4 then begin
                            s_paredes := s_paredes+'"sur"';
                            mundo[i,j].pared := mundo[i,j].pared-4;
                        end;
                        if k = 2 then begin
                            s_paredes := s_paredes+'"este"';
                            mundo[i,j].pared := mundo[i,j].pared-2;
                        end;
                        if k = 1 then begin
                            s_paredes := s_paredes+'"norte"';
                            mundo[i,j].pared := mundo[i,j].pared-1;
                        end;
                        c:=1;
                    end;
                    k := k/2;
                end;
                writeln('            "paredes": [',s_paredes,'],');
                writeln('            "fila": ',i,',');
                writeln('            "columna": ',j);
                writeln('        }');
                contador:=1;
            end;
        end;
    end;
    //fin casillas
    writeln('    ],');
    writeln('    "dimensiones": {');
    writeln('        "columnas": ', ancho,',');
    writeln('        "filas": ',alto);
    writeln('    }');
    writeln('}');

    close(f);
end.
