start mul val1
	ldb #2
	addr A B
	float
loop lda #1
	sta val2
	j loop
val1 word 12
val2 word 0