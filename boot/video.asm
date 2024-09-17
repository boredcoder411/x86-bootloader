; This inits 640x480 16 color graphics
[bits 16]
init_video_graphics:
	mov ah, 0x00
	mov al, 0x13
	int 0x10
	ret

; Note to self: this inits text mode, granted, but it also clears the screen
[bits 16]
init_video_text:
	;mov ah, 0x00
	;mov al, 0x03
	;int 0x10
	ret
