procedure player_sprite3_0();
begin
asm ("
  ld de, $20
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $0a
 ld [iy+1],a
 ld a,[ix+2] 
 or $a0
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $26
 ld [iy+1],a
 ld a,[ix+2] 
 or $68
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $2a
 ld [iy+1],a
 ld a,[ix+2] 
 or $a8
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $0a
 ld [iy+1],a
 ld a,[ix+2] 
 or $a0
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $08
 ld [iy+1],a
 ld a,[ix+2] 
 or $20
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
");
end;
procedure player_sprite3_1();
begin
asm ("
  ld de, $20
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $02
 ld [iy+1],a
 ld a,[ix+2] 
 or $a8
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $09
 ld [iy+1],a
 ld a,[ix+2] 
 or $9a
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $0a
 ld [iy+1],a
 ld a,[ix+2] 
 or $aa
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $02
 ld [iy+1],a
 ld a,[ix+2] 
 or $a8
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $02
 ld [iy+1],a
 ld a,[ix+2] 
 or $08
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
");
end;
procedure player_sprite3_2();
begin
asm ("
  ld de, $20
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 or $aa
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $02
 ld [iy+1],a
 ld a,[ix+2] 
 or $66
 ld [iy+2],a
 ld a,[ix+3] 
 or $80
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 or $02
 ld [iy+1],a
 ld a,[ix+2] 
 or $aa
 ld [iy+2],a
 ld a,[ix+3] 
 or $80
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 or $aa
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 or $82
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
");
end;
procedure player_sprite3_3();
begin
asm ("
  ld de, $20
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 or $2a
 ld [iy+2],a
 ld a,[ix+3] 
 or $80
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 or $99
 ld [iy+2],a
 ld a,[ix+3] 
 or $a0
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 or $aa
 ld [iy+2],a
 ld a,[ix+3] 
 or $a0
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 or $2a
 ld [iy+2],a
 ld a,[ix+3] 
 or $80
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 or $20
 ld [iy+2],a
 ld a,[ix+3] 
 or $80
 ld [iy+3],a
  add iy,de
  add ix,de
 ld a,[ix+0] 
 ld [iy+0],a
 ld a,[ix+1] 
 ld [iy+1],a
 ld a,[ix+2] 
 ld [iy+2],a
 ld a,[ix+3] 
 ld [iy+3],a
  add iy,de
  add ix,de
");
end;
var player_sprite3_pi : integer;
var player_sprite3_buf : integer;
@donotremove player_sprite3_dst 
procedure player_sprite3(player_sprite3_src,player_sprite3_dst : pointer; player_sprite3px,player_sprite3py : byte);
begin
  player_sprite3_pi:=player_sprite3_dst + player_sprite3py<<5 + player_sprite3px>>2;
  player_sprite3_buf:=#buf + player_sprite3py<<5 + player_sprite3px>>2;
asm("  ld ix, [player_sprite3_buf] 
  ld iy, [player_sprite3_pi]
");
 player_sprite3px:=player_sprite3px&3;
 if (player_sprite3px=0) then player_sprite3_0();
 if (player_sprite3px=1) then player_sprite3_1();
 if (player_sprite3px=2) then player_sprite3_2();
 if (player_sprite3px=3) then player_sprite3_3();
end;
