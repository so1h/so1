	.286
	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	V 301h
	?debug	S "contkd.c"
	?debug	C E9846E6D4C08636F6E746B642E63
	?debug	C E9CF81654C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C616A7573747573722E68
	?debug	C E9149D71481F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C66696E636F64652E68
	?debug	C E9149D714821583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7374617274646174612E68
	?debug	C E9149D71481F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C66696E646174612E68
	?debug	C E9149D71481E583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C66696E6273732E68
	?debug	C E9D5BE664C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C636F6D756E6472762E68
	?debug	C E9CF81654C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C616A7573747573722E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9A088164B1B583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6332632E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9149D71481E583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7074726332632E68
	?debug	C E9A088164B1B583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6332632E68
	?debug	C E9ACBC634C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F70726F632E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E92FBE554C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C636F6E73742E68
	?debug	C E95CB65A4A1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C66636E746C2E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9149D71481F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F74726D2E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9149D71481F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7363616E6E65722E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9A088164B1B583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6332632E68
	?debug	C E926A6664C1B583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6363622E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
_TEXT	segment byte public 'CODE'
_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_DATA	segment word public 'DATA'
_ptrIndProcesoActual	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrTramaProceso	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrActivarAlEpilogo	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrDescProceso	label	dword
	db	0
	db	0
	db	0
	db	0
_tamDescProceso	label	word
	db	0
	db	0
_ptrDescFichero	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrDescRecurso	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrC2cPFR	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrNivelActivacionSO1	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrEnHalt	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrHayTic	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrCcbAlEpilogo	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrSS_Proceso	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrSP_Proceso	label	dword
	db	0
	db	0
	db	0
	db	0
_SP0_SO1	label	word
	db	0
	db	0
_ptrContRodajas	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrContTicsRodaja	label	dword
	db	0
	db	0
	db	0
	db	0
_ptrVIOrg	label	dword
	db	0
	db	0
	db	0
	db	0
_sigProceso	label	dword
	db	0
	db	0
	db	0
	db	0
_activarProceso	label	dword
	db	0
	db	0
	db	0
	db	0
_buscarNuevoProcesoActual	label	dword
	db	0
	db	0
	db	0
	db	0
_bloquearProcesoActual	label	dword
	db	0
	db	0
	db	0
	db	0
	?debug	C E9B9126B4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6C6C5F735F736F312E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9ACBC634C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F70726F632E68
	?debug	C E9FB06FC461F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F6D656D2E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E93DA1554C1E583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F73662E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E90871694C1F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F7261742E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9F6036A4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F70616E742E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9450B664C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C62696F73646174612E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E95CB65A4A1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C66636E746C2E68
	?debug	C E9CD01534C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6C6C5F735F6578632E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9ACBC634C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F70726F632E68
	?debug	C E9F186524C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6C6C5F735F7369672E68
	?debug	C E921BF4B4C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C737464696F2E68
	?debug	C E90E885A4C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C63747970652E68
	?debug	C E9C7705B4C1F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C737472696E67732E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E97400554C1E583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C62696F735F302E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E979B4694C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C62696F735F6372742E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9E4106A4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C70616E74616C6C612E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9F6036A4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F70616E742E68
	?debug	C E9342D45471D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C63727468772E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9C8066A4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6D656D766964656F2E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9E4106A4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C70616E74616C6C612E68
	?debug	C E96DB0694C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F7465636C2E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9149D71481E583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7074726332632E68
	?debug	C E9149D714820583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F7363616E2E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E92FBE554C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C636F6E73742E68
	?debug	C E9450B664C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C62696F73646174612E68
	?debug	C E9B900484A1B583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7069632E68
	?debug	C E9FB06FC461F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C70756572746F732E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E902A5664C21583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F74696D65722E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9D29B5A4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7072696E7467656E2E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9149D71481D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C64656275672E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9149D714821583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7374617274646174612E68
	?debug	C E9149D714820583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C73746172746273732E68
	?debug	C E9149D71481E583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C66696E6273732E68
	?debug	C E90AAD6C4C26583A5C534F5C5052414354305C70726F677573725C+
	?debug	C 636F6E736F6C615C636F6E736F6C612E68
	?debug	C E96DB0694C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F7465636C2E68
	?debug	C E9F6036A4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F70616E742E68
	?debug	C E94CBD684C26583A5C534F5C5052414354305C70726F677573725C+
	?debug	C 636F6E736F6C615C7461626C6173702E68
_tablaSP	label	byte
	db	0
	db	1
	db	2
	db	3
	db	4
	db	5
	db	6
	db	7
	db	8
	db	9
	db	10
	db	11
	db	12
	db	13
	db	14
	db	15
	db	16
	db	17
	db	18
	db	19
	db	20
	db	21
	db	22
	db	23
	db	24
	db	25
	db	26
	db	27
	db	28
	db	29
	db	30
	db	31
	db	32
	db	33
	db	34
	db	183
	db	36
	db	37
	db	47
	db	39
	db	41
	db	61
	db	40
	db	191
	db	44
	db	39
	db	46
	db	45
	db	48
	db	49
	db	50
	db	51
	db	52
	db	53
	db	54
	db	55
	db	56
	db	57
	db	58
	db	59
	db	59
	db	161
	db	58
	db	95
	db	34
	db	65
	db	66
	db	67
	db	68
	db	69
	db	70
	db	71
	db	72
	db	73
	db	74
	db	75
	db	76
	db	77
	db	78
	db	79
	db	80
	db	81
	db	82
	db	83
	db	84
	db	85
	db	86
	db	87
	db	88
	db	89
	db	90
	db	91
	db	60
	db	43
	db	38
	db	63
	db	186
	db	97
	db	98
	db	99
	db	100
	db	101
	db	102
	db	103
	db	104
	db	105
	db	106
	db	107
	db	108
	db	109
	db	110
	db	111
	db	112
	db	113
	db	114
	db	115
	db	116
	db	117
	db	118
	db	119
	db	120
	db	121
	db	122
	db	123
	db	62
	db	42
	db	170
	db	127
	db	128
	db	129
	db	130
	db	131
	db	132
	db	133
	db	134
	db	135
	db	136
	db	137
	db	138
	db	139
	db	140
	db	141
	db	142
	db	143
	db	144
	db	145
	db	146
	db	147
	db	148
	db	149
	db	150
	db	151
	db	152
	db	153
	db	154
	db	155
	db	156
	db	157
	db	158
	db	159
	db	160
	db	161
	db	162
	db	163
	db	164
	db	165
	db	166
	db	167
	db	168
	db	169
	db	170
	db	171
	db	172
	db	173
	db	174
	db	175
	db	176
	db	177
	db	178
	db	179
	db	180
	db	181
	db	182
	db	183
	db	184
	db	185
	db	186
	db	187
	db	188
	db	189
	db	190
	db	191
	db	192
	db	193
	db	194
	db	195
	db	196
	db	197
	db	198
	db	199
	db	200
	db	201
	db	202
	db	203
	db	204
	db	205
	db	206
	db	207
	db	208
	db	209
	db	210
	db	211
	db	212
	db	213
	db	214
	db	215
	db	216
	db	217
	db	218
	db	219
	db	220
	db	221
	db	222
	db	223
	db	224
	db	225
	db	226
	db	227
	db	228
	db	229
	db	230
	db	231
	db	232
	db	233
	db	234
	db	235
	db	236
	db	237
	db	238
	db	239
	db	240
	db	241
	db	242
	db	243
	db	244
	db	245
	db	246
	db	247
	db	248
	db	249
	db	250
	db	251
	db	252
	db	253
	db	254
	db	255
_rec_consola	label	word
	db	0
	db	0
_descConsola	label	dword
	db	0
	db	0
	db	0
	db	0
_maxConsolas	label	byte
	db	0
	db	1 dup (0)
_consolaDeSuperficie	label	word
	db	0
	db	0
_nbytesProceso	label	word
	db	0
	db	0
	db	30 dup (0)
_dirProceso	label	dword
	db	0
	db	0
	db	0
	db	0
	db	60 dup (0)
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_meter	proc	near
	?debug	C E80108636F6E746B642E63846E6D4C
	?debug	C E82820583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C616A7573747573722E68CF81654C
	?debug	C E8021F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66696E636F64652E68149D7148
	?debug	C E828
	?debug	C E80321583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7374617274646174612E68149D7148
	?debug	C E828
	?debug	C E8041F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66696E646174612E68149D7148
	?debug	C E828
	?debug	C E8051E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66696E6273732E68149D7148
	?debug	C E828
	?debug	C E801
	?debug	C E80E20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C636F6D756E6472762E68D5BE664C
	?debug	C E82920583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C616A7573747573722E68CF81654C
	?debug	C E80E
	?debug	C E8061D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80E
	?debug	C E8071B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6332632E68A088164B
	?debug	C E82A1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E807
	?debug	C E80E
	?debug	C E8081E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7074726332632E68149D7148
	?debug	C E82B1B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6332632E68A088164B
	?debug	C E808
	?debug	C E80E
	?debug	C E80D20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70726F632E68ACBC634C
	?debug	C E82C1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80D
	?debug	C E82D1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C636F6E73742E682FBE554C
	?debug	C E80D
	?debug	C E8091D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66636E746C2E685CB65A4A
	?debug	C E82E1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E809
	?debug	C E80D
	?debug	C E80A1F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F74726D2E68149D7148
	?debug	C E82F1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80A
	?debug	C E80D
	?debug	C E80B1F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7363616E6E65722E68149D7148
	?debug	C E8301D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80B
	?debug	C E80D
	?debug	C E8311B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6332632E68A088164B
	?debug	C E80D
	?debug	C E80C1B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6363622E6826A6664C
	?debug	C E8321D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80C
	?debug	C E80D
	?debug	C E80E
	?debug	C E801
	?debug	C E81620583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6C6C5F735F736F312E68B9126B4C
	?debug	C E8331D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E816
	?debug	C E83420583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70726F632E68ACBC634C
	?debug	C E816
	?debug	C E80F1F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F6D656D2E68FB06FC46
	?debug	C E8351D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80F
	?debug	C E816
	?debug	C E8101E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F73662E683DA1554C
	?debug	C E8361D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E810
	?debug	C E816
	?debug	C E8131F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F7261742E680871694C
	?debug	C E8371D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E813
	?debug	C E81220583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70616E742E68F6036A4C
	?debug	C E8381D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E812
	?debug	C E81120583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C62696F73646174612E68450B664C
	?debug	C E8391D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E811
	?debug	C E812
	?debug	C E813
	?debug	C E816
	?debug	C E83A1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66636E746C2E685CB65A4A
	?debug	C E816
	?debug	C E81420583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6C6C5F735F6578632E68CD01534C
	?debug	C E83B1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E814
	?debug	C E83C20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70726F632E68ACBC634C
	?debug	C E814
	?debug	C E816
	?debug	C E81520583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6C6C5F735F7369672E68F186524C
	?debug	C E816
	?debug	C E801
	?debug	C E8171D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C737464696F2E6821BF4B4C
	?debug	C E801
	?debug	C E8181D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C63747970652E680E885A4C
	?debug	C E801
	?debug	C E8191F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C737472696E67732E68C7705B4C
	?debug	C E83D1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E819
	?debug	C E801
	?debug	C E81A1E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C62696F735F302E687400554C
	?debug	C E83E1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81A
	?debug	C E801
	?debug	C E81C20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C62696F735F6372742E6879B4694C
	?debug	C E83F1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81C
	?debug	C E81B20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C70616E74616C6C612E68E4106A4C
	?debug	C E8401D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81B
	?debug	C E84120583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70616E742E68F6036A4C
	?debug	C E81B
	?debug	C E81C
	?debug	C E801
	?debug	C E81D1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C63727468772E68342D4547
	?debug	C E8421D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81D
	?debug	C E801
	?debug	C E81E20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6D656D766964656F2E68C8066A4C
	?debug	C E8431D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81E
	?debug	C E84420583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C70616E74616C6C612E68E4106A4C
	?debug	C E81E
	?debug	C E801
	?debug	C E81F20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F7465636C2E686DB0694C
	?debug	C E8451D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81F
	?debug	C E8461E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7074726332632E68149D7148
	?debug	C E81F
	?debug	C E84720583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F7363616E2E68149D7148
	?debug	C E8481D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E847
	?debug	C E81F
	?debug	C E8491D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C636F6E73742E682FBE554C
	?debug	C E81F
	?debug	C E84A20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C62696F73646174612E68450B664C
	?debug	C E81F
	?debug	C E801
	?debug	C E8211B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7069632E68B900484A
	?debug	C E8201F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C70756572746F732E68FB06FC46
	?debug	C E84B1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E820
	?debug	C E821
	?debug	C E84C1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E821
	?debug	C E801
	?debug	C E82221583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F74696D65722E6802A5664C
	?debug	C E84D1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E822
	?debug	C E801
	?debug	C E82320583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7072696E7467656E2E68D29B5A4C
	?debug	C E84E1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E823
	?debug	C E801
	?debug	C E8251D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C64656275672E68149D7148
	?debug	C E84F1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E825
	?debug	C E85021583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7374617274646174612E68149D7148
	?debug	C E825
	?debug	C E82420583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C73746172746273732E68149D7148
	?debug	C E825
	?debug	C E8511E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66696E6273732E68149D7148
	?debug	C E825
	?debug	C E801
	?debug	C E82626583A5C534F5C5052414354305C70726F67+
	?debug	C 7573725C636F6E736F6C615C636F6E736F6C612E+
	?debug	C 680AAD6C4C
	?debug	C E85220583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F7465636C2E686DB0694C
	?debug	C E826
	?debug	C E85320583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70616E742E68F6036A4C
	?debug	C E826
	?debug	C E801
	?debug	C E82726583A5C534F5C5052414354305C70726F67+
	?debug	C 7573725C636F6E736F6C615C7461626C6173702E+
	?debug	C 684CBD684C
	?debug	C E801
   ;	
   ;	void meter ( char car, teclado_t far * teclado )
   ;	
	enter	2,0
   ;	
   ;	{
   ;	    int in = teclado->in ;
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr es:[bx+338]
	mov	word ptr [bp-2],ax
   ;	
   ;	    if (teclado->ncar < tamBTeclado)
   ;	
	les	bx,dword ptr [bp+6]
	cmp	word ptr es:[bx+342],256
	jge	short @1@5
   ;	
   ;	    {
   ;	        teclado->bufer[in++] = car ;
   ;	
	les	bx,dword ptr [bp+6]
	add	bx,word ptr [bp-2]
	mov	al,byte ptr [bp+4]
	mov	byte ptr es:[bx+82],al
	inc	word ptr [bp-2]
   ;	
   ;	        if (in >= tamBTeclado) in = 0 ;
   ;	
	cmp	word ptr [bp-2],256
	jl	short @1@4
	mov	word ptr [bp-2],0
@1@4:
   ;	
   ;	        teclado->in = in ;
   ;	
	les	bx,dword ptr [bp+6]
	mov	ax,word ptr [bp-2]
	mov	word ptr es:[bx+338],ax
   ;	
   ;	        teclado->ncar++ ;
   ;	
	les	bx,dword ptr [bp+6]
	inc	word ptr es:[bx+342]
@1@5:
   ;	
   ;	    }
   ;	}
   ;	
	leave	
	ret	
_meter	endp
	assume	cs:_TEXT,ds:DGROUP
_sacar	proc	near
   ;	
   ;	char sacar ( teclado_t far * teclado  )     /* se supone: teclado.ncar > 0 */
   ;	
	enter	4,0
   ;	
   ;	{
   ;	    char car ;
   ;	    int out = teclado->out ;
   ;	
	les	bx,dword ptr [bp+4]
	mov	ax,word ptr es:[bx+340]
	mov	word ptr [bp-4],ax
   ;	
   ;	    car = teclado->bufer[out++] ;
   ;	
	les	bx,dword ptr [bp+4]
	add	bx,word ptr [bp-4]
	mov	al,byte ptr es:[bx+82]
	mov	byte ptr [bp-1],al
	inc	word ptr [bp-4]
   ;	
   ;	    if (out >= tamBTeclado) out = 0 ;
   ;	
	cmp	word ptr [bp-4],256
	jl	short @2@3
	mov	word ptr [bp-4],0
@2@3:
   ;	
   ;	    teclado->out = out ;
   ;	
	les	bx,dword ptr [bp+4]
	mov	ax,word ptr [bp-4]
	mov	word ptr es:[bx+340],ax
   ;	
   ;	    teclado->ncar-- ;
   ;	
	les	bx,dword ptr [bp+4]
	dec	word ptr es:[bx+342]
   ;	
   ;	    return(car) ;
   ;	
	mov	al,byte ptr [bp-1]
   ;	
   ;	}
   ;	
	leave	
	ret	
_sacar	endp
	assume	cs:_TEXT,ds:DGROUP
_printCarConsola	proc	near
   ;	
   ;	int printCarConsola ( byte_t con, char car )
   ;	
	enter	8,0
   ;	
   ;	{
   ;	    if (con == consolaDeSuperficie)
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	cmp	ax,word ptr DGROUP:_consolaDeSuperficie
	jne	short @3@3
   ;	
   ;	        printCarVideo(car) ;
   ;	
	mov	al,byte ptr [bp+6]
	push	ax
	call	near ptr _printCarVideo
	pop	cx
	jmp	@3@22
@3@3:
   ;	
   ;	    else
   ;	#if (0) 			
   ;			printCarDisplay((display_t far *)&(descConsola[con]), car) ;
   ;	#else	
   ;	    {
   ;		    byte_t numFilas = maxFilasAct ;
   ;	
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	inc	al
	mov	byte ptr [bp-1],al
   ;	
   ;	        pantalla_t far * pantalla = (pantalla_t far *)&descConsola[con].pantalla ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola+2
	mov	bx,word ptr DGROUP:_descConsola
	add	bx,ax
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-6],bx
   ;	
   ;	        byte_t F = descConsola[con].F ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr es:[bx+8000]
	mov	byte ptr [bp-7],al
   ;	
   ;	        byte_t C = descConsola[con].C ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr es:[bx+8001]
	mov	byte ptr [bp-8],al
   ;	
   ;	        switch (car)
   ;	
	mov	al,byte ptr [bp+6]
	cbw	
	sub	ax,7
	mov	bx,ax
	cmp	bx,6
	jbe	@@0
	jmp	@3@19
@@0:
	add	bx,bx
	jmp	word ptr cs:@3@C62[bx]
@3@7:
   ;	
   ;	        {
   ;	        case '\f'  :
   ;	            borrarCPantalla(pantalla, numFilas) ;
   ;	
	mov	al,byte ptr [bp-1]
	mov	ah,0
	push	ax
	push	word ptr [bp-4]
	push	word ptr [bp-6]
	call	near ptr _borrarCPantalla
	add	sp,6
   ;	
   ;	//          borrarPantalla(pantalla, numFilas) ; 
   ;	            descConsola[con].F = 0 ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	byte ptr es:[bx+8000],0
   ;	
   ;	            descConsola[con].C = 0 ;
   ;	
@3@8:
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	byte ptr es:[bx+8001],0
   ;	
   ;	            break ;
   ;	
	jmp	@3@22
@3@9:
	jmp	short @3@8
@3@10:
   ;	
   ;	        case '\r'  :
   ;	            descConsola[con].C = 0 ;
   ;	            break ;
   ;	        case '\n'  :
   ;	            if (++descConsola[con].F == numFilas)
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr es:[bx+8000]
	inc	al
	mov	dl,byte ptr [bp+4]
	mov	dh,0
	imul	dx,dx,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,dx
	mov	byte ptr es:[bx+8000],al
	cmp	al,byte ptr [bp-1]
	jne	short @3@12
   ;	
   ;	            {
   ;	                scrollCPantalla(pantalla, numFilas) ;
   ;	
	mov	al,byte ptr [bp-1]
	mov	ah,0
	push	ax
	push	word ptr [bp-4]
	push	word ptr [bp-6]
	call	near ptr _scrollCPantalla
	add	sp,6
   ;	
   ;	                descConsola[con].F = numFilas-1 ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr [bp-1]
	add	al,255
	mov	byte ptr es:[bx+8000],al
@3@12:
   ;	
   ;	            }
   ;	            break ;
   ;	
	jmp	@3@22
@3@13:
   ;	
   ;	        case '\b'  :
   ;	            if (C > 0)
   ;	
	cmp	byte ptr [bp-8],0
	jbe	short @3@15
   ;	
   ;	            {
   ;	                C-- ;
   ;	
	dec	byte ptr [bp-8]
   ;	
   ;	                pantalla->t[F][C].car = ' ' ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,160
	les	bx,dword ptr [bp-6]
	add	bx,ax
	mov	al,byte ptr [bp-8]
	mov	ah,0
	add	ax,ax
	add	bx,ax
	mov	byte ptr es:[bx],32
   ;	
   ;	                descConsola[con].C = C ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr [bp-8]
	mov	byte ptr es:[bx+8001],al
   ;	
   ;	            }
   ;	
	jmp	short @3@16
@3@15:
   ;	
   ;	            else printf("\a") ;
   ;	
	push	ds
	push	offset DGROUP:s@
	call	near ptr _printf
	add	sp,4
@3@16:
   ;	
   ;	            break ;
   ;	
	jmp	@3@22
@3@17:
   ;	
   ;	        case '\t'  :
   ;	            car = ' ' ;
   ;	
	mov	byte ptr [bp+6],32
@3@18:
   ;	
   ;	        case '\a' :
   ;	            printf("\a") ;
   ;	
	push	ds
	push	offset DGROUP:s@+2
	call	near ptr _printf
	add	sp,4
   ;	
   ;	            break ;
   ;	
	jmp	@3@22
@3@19:
   ;	
   ;	        default  :
   ;	#if (FALSE)
   ;	            if (C < 80)                      /* ptrBiosArea->VIDEO-width */
   ;	                pantalla->t[F][C].car = car ;
   ;	            descConsola[con].C++ ;
   ;	#else
   ;	            pantalla->t[F][C].car = car ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,160
	les	bx,dword ptr [bp-6]
	add	bx,ax
	mov	al,byte ptr [bp-8]
	mov	ah,0
	add	ax,ax
	add	bx,ax
	mov	al,byte ptr [bp+6]
	mov	byte ptr es:[bx],al
   ;	
   ;	            if (++descConsola[con].C == 80)  /* ptrBiosArea->VIDEO-width */
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr es:[bx+8001]
	inc	al
	mov	dl,byte ptr [bp+4]
	mov	dh,0
	imul	dx,dx,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,dx
	mov	byte ptr es:[bx+8001],al
	cmp	al,80
	jne	short @3@22
   ;	
   ;	            {
   ;	                descConsola[con].C = 0 ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	byte ptr es:[bx+8001],0
   ;	
   ;	                if (++descConsola[con].F == numFilas)
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr es:[bx+8000]
	inc	al
	mov	dl,byte ptr [bp+4]
	mov	dh,0
	imul	dx,dx,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,dx
	mov	byte ptr es:[bx+8000],al
	cmp	al,byte ptr [bp-1]
	jne	short @3@22
   ;	
   ;	                {
   ;	                    scrollCPantalla(pantalla, numFilas) ;
   ;	
	mov	al,byte ptr [bp-1]
	mov	ah,0
	push	ax
	push	word ptr [bp-4]
	push	word ptr [bp-6]
	call	near ptr _scrollCPantalla
	add	sp,6
   ;	
   ;	                    descConsola[con].F = numFilas-1 ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr [bp-1]
	add	al,255
	mov	byte ptr es:[bx+8000],al
@3@22:
   ;	
   ;	                }
   ;	            }
   ;	#endif
   ;	        }
   ;	    }
   ;	#endif		
   ;	    return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	leave	
	ret	
_printCarConsola	endp
@3@C62	label	word
	dw	@3@18
	dw	@3@13
	dw	@3@17
	dw	@3@10
	dw	@3@19
	dw	@3@7
	dw	@3@9
	assume	cs:_TEXT,ds:DGROUP
_goToXYConsola	proc	near
   ;	
   ;	int goToXYConsola ( byte_t con, byte_t F, byte_t C )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    if (F >= maxFilasAct) return(-1) ;
   ;	
	mov	al,byte ptr [bp+6]
	mov	ah,0
	xor	dx,dx
	mov	es,dx
	mov	dl,byte ptr es:[1156]
	mov	dh,0
	inc	dx
	cmp	ax,dx
	jl	short @4@3
	jmp	short @4@4
@4@3:
   ;	
   ;	    if (C >= maxColumnasAct) return(-1) ;  
   ;	
	mov	al,byte ptr [bp+8]
	mov	ah,0
	xor	dx,dx
	mov	es,dx
	cmp	ax,word ptr es:[1098]
	jb	short @4@5
@4@4:
	mov	ax,-1
	pop	bp
	ret	
@4@5:
   ;	
   ;	    if (con == consolaDeSuperficie)
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	cmp	ax,word ptr DGROUP:_consolaDeSuperficie
	jne	short @4@7
   ;	
   ;	    {
   ;	//      goToXYHw(F, C) ; 
   ;	        cursorF = F ;
   ;	
	mov	al,byte ptr [bp+6]
	mov	byte ptr DGROUP:_cursorF,al
   ;	
   ;	        cursorC = C ;
   ;	
	mov	al,byte ptr [bp+8]
	mov	byte ptr DGROUP:_cursorC,al
   ;	
   ;	    }
   ;	
	jmp	short @4@8
@4@7:
   ;	
   ;	    else
   ;	    {
   ;	        descConsola[con].F = F ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr [bp+6]
	mov	byte ptr es:[bx+8000],al
   ;	
   ;	        descConsola[con].C = C ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr [bp+8]
	mov	byte ptr es:[bx+8001],al
@4@8:
   ;	
   ;	    }
   ;	    return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_goToXYConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_establecerConsolaDeSuperficie	proc	near
   ;	
   ;	void establecerConsolaDeSuperficie ( void )
   ;	
	enter	4,0
   ;	
   ;	{
   ;	    pantalla_t far * pantalla =
   ;	        (pantalla_t far *)&descConsola[consolaDeSuperficie].pantalla ;
   ;	
	mov	ax,word ptr DGROUP:_consolaDeSuperficie
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola+2
	mov	bx,word ptr DGROUP:_descConsola
	add	bx,ax
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],bx
   ;	
   ;	    copiarCPantalla(pantalla, ptrPant, maxFilasAct) ;
   ;	
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	push	ax
	push	word ptr DGROUP:_ptrPant+2
	push	word ptr DGROUP:_ptrPant
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	call	near ptr _copiarCPantalla
	add	sp,10
   ;	
   ;	//  copiarPantalla(pantalla, ptrPant, maxFilasAct) ; 
   ;	    cursorF = descConsola[consolaDeSuperficie].F ;
   ;	
	mov	ax,word ptr DGROUP:_consolaDeSuperficie
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr es:[bx+8000]
	mov	byte ptr DGROUP:_cursorF,al
   ;	
   ;	    cursorC = descConsola[consolaDeSuperficie].C ;
   ;	
	mov	ax,word ptr DGROUP:_consolaDeSuperficie
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr es:[bx+8001]
	mov	byte ptr DGROUP:_cursorC,al
   ;	
   ;	    goToXYHw(cursorF, cursorC) ;
   ;	
	mov	al,byte ptr DGROUP:_cursorC
	push	ax
	mov	al,byte ptr DGROUP:_cursorF
	push	ax
	call	near ptr _goToXYHw
	add	sp,4
   ;	
   ;	}
   ;	
	leave	
	ret	
_establecerConsolaDeSuperficie	endp
	assume	cs:_TEXT,ds:DGROUP
_salvarConsolaDeSuperficie	proc	near
   ;	
   ;	void salvarConsolaDeSuperficie ( void )
   ;	
	enter	4,0
   ;	
   ;	{
   ;	    pantalla_t far * pantalla =
   ;	        (pantalla_t far *)&descConsola[consolaDeSuperficie].pantalla ;
   ;	
	mov	ax,word ptr DGROUP:_consolaDeSuperficie
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola+2
	mov	bx,word ptr DGROUP:_descConsola
	add	bx,ax
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],bx
   ;	
   ;	    copiarCPantalla(ptrPant, pantalla, maxFilasAct) ;
   ;	
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	push	ax
	push	word ptr [bp-2]
	push	word ptr [bp-4]
	push	word ptr DGROUP:_ptrPant+2
	push	word ptr DGROUP:_ptrPant
	call	near ptr _copiarCPantalla
	add	sp,10
   ;	
   ;	//  copiarPantalla(ptrPant, pantalla, maxFilasAct) ;
   ;	    descConsola[consolaDeSuperficie].F = cursorF ;
   ;	
	mov	ax,word ptr DGROUP:_consolaDeSuperficie
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr DGROUP:_cursorF
	mov	byte ptr es:[bx+8000],al
   ;	
   ;	    descConsola[consolaDeSuperficie].C = cursorC ;
   ;	
	mov	ax,word ptr DGROUP:_consolaDeSuperficie
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr DGROUP:_cursorC
	mov	byte ptr es:[bx+8001],al
   ;	
   ;	}
   ;	
	leave	
	ret	
_salvarConsolaDeSuperficie	endp
	assume	cs:_TEXT,ds:DGROUP
_openConsola	proc	far
   ;	
   ;	int far openConsola ( int dfs, modoAp_t modo )
   ;	
   ;	
   ;	{
   ;	    return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	ret	
_openConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_releaseConsola	proc	far
   ;	
   ;	int far releaseConsola ( int dfs )
   ;	
   ;	
   ;	{
   ;	    return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	ret	
_releaseConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_readConsola	proc	far
   ;	
   ;	int far readConsola ( int dfs, pointer_t dir, word_t nbytes )
   ;	
	enter	18,0
   ;	
   ;	{
   ;	    pindx_t indProcesoActual ;
   ;	    teclado_t far * teclado ;
   ;	    modoAp_t modoAp ;
   ;	    word_t nbytes0 ;
   ;	    int df ;
   ;	    int i = 0 ;
   ;	
	mov	word ptr [bp-14],0
   ;	
   ;	    char car ;
   ;	    word_t con ;
   ;	
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	    setraw_DS() ;           /* establece el DS correspondiente al programa */
   ;	
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
   ;	
   ;	
   ;	    indProcesoActual = *ptrIndProcesoActual ;
   ;	
	les	bx,dword ptr DGROUP:_ptrIndProcesoActual
	mov	ax,word ptr es:[bx]
	mov	word ptr [bp-2],ax
   ;	
   ;	    df = (*ptrTramaProceso)->BX ;
   ;	
	les	bx,dword ptr DGROUP:_ptrTramaProceso
	les	bx,dword ptr es:[bx]
	mov	ax,word ptr es:[bx+12]
	mov	word ptr [bp-12],ax
   ;	
   ;	    con = ptrDescFichero[dfs].menor ;
   ;	
	mov	ax,word ptr [bp+6]
	imul	ax,ax,21
	les	bx,dword ptr DGROUP:_ptrDescFichero
	add	bx,ax
	mov	ax,word ptr es:[bx+13]
	mov	word ptr [bp-18],ax
   ;	
   ;	    teclado = (teclado_t far *)&descConsola[con].teclado ;
   ;	
	mov	ax,word ptr [bp-18]
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola+2
	mov	bx,word ptr DGROUP:_descConsola
	add	bx,ax
	add	bx,8002
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-6],bx
   ;	
   ;	    modoAp = ptrDescProceso[indProcesoActual].tfa[df].modoAp ;
   ;	
	mov	ax,word ptr [bp-2]
	imul	ax,ax,220
	les	bx,dword ptr DGROUP:_ptrDescProceso
	add	bx,ax
	mov	ax,word ptr [bp-12]
	shl	ax,3
	add	bx,ax
	mov	ax,word ptr es:[bx+136]
	mov	word ptr [bp-8],ax
   ;	
   ;	
   ;	    nbytes0 = nbytes ;
   ;	
	mov	ax,word ptr [bp+12]
	mov	word ptr [bp-10],ax
   ;	
   ;	    if (nbytes <= teclado->ncar)
   ;	
	les	bx,dword ptr [bp-6]
	mov	ax,word ptr es:[bx+342]
	cmp	ax,word ptr [bp+12]
	jb	short @9@17
	jmp	short @9@13
@9@9:
   ;	
   ;	    {
   ;	        while (nbytes > 0)
   ;	        {
   ;	//          car = sacar(teclado) ;
   ;	//          car = tablaSP[sacar(teclado)] ;
   ;	            car = tablaDeConversion[sacar(teclado)] ;
   ;	
	push	word ptr [bp-4]
	push	word ptr [bp-6]
	call	near ptr _sacar
	add	sp,4
	cbw	
	mov	bx,ax
	mov	al,byte ptr DGROUP:_tablaSP[bx]
	mov	byte ptr [bp-15],al
   ;	
   ;	            if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
   ;	
	cmp	byte ptr [bp-15],13
	jne	short @9@12
	test	word ptr [bp-8],16384
	je	short @9@12
	mov	byte ptr [bp-15],10
@9@12:
   ;	
   ;	            dir[i++] = car ;
   ;	
	les	bx,dword ptr [bp+8]
	add	bx,word ptr [bp-14]
	mov	al,byte ptr [bp-15]
	mov	byte ptr es:[bx],al
	inc	word ptr [bp-14]
   ;	
   ;	            nbytes-- ;
   ;	
	dec	word ptr [bp+12]
@9@13:
	cmp	word ptr [bp+12],0
	ja	short @9@9
   ;	
   ;	        }
   ;	        restore_DS0() ;                           /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	        return(nbytes0) ;
   ;	
	mov	ax,word ptr [bp-10]
	leave	
	ret	
   ;	
   ;	    }
   ;	
	jmp	@9@24
@9@17:
	jmp	short @9@22
@9@18:
   ;	
   ;	    else
   ;	    {
   ;	        while (teclado->ncar > 0)
   ;	        {
   ;	//          car = sacar(teclado) ;
   ;	//          car = tablaSP[sacar(teclado)] ;
   ;	            car = tablaDeConversion[sacar(teclado)] ;
   ;	
	push	word ptr [bp-4]
	push	word ptr [bp-6]
	call	near ptr _sacar
	add	sp,4
	cbw	
	mov	bx,ax
	mov	al,byte ptr DGROUP:_tablaSP[bx]
	mov	byte ptr [bp-15],al
   ;	
   ;	            if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
   ;	
	cmp	byte ptr [bp-15],13
	jne	short @9@21
	test	word ptr [bp-8],16384
	je	short @9@21
	mov	byte ptr [bp-15],10
@9@21:
   ;	
   ;	            dir[i++] = car ;
   ;	
	les	bx,dword ptr [bp+8]
	add	bx,word ptr [bp-14]
	mov	al,byte ptr [bp-15]
	mov	byte ptr es:[bx],al
	inc	word ptr [bp-14]
   ;	
   ;	            nbytes-- ;
   ;	
	dec	word ptr [bp+12]
@9@22:
	les	bx,dword ptr [bp-6]
	cmp	word ptr es:[bx+342],0
	jg	short @9@18
   ;	
   ;	        }
   ;	        nbytesProceso[indProcesoActual] = nbytes ;
   ;	
	mov	bx,word ptr [bp-2]
	add	bx,bx
	mov	ax,word ptr [bp+12]
	mov	word ptr DGROUP:_nbytesProceso[bx],ax
   ;	
   ;	        dirProceso[indProcesoActual] = (pointer_t)&dir[i] ;
   ;	
	mov	dx,word ptr [bp+10]
	mov	ax,word ptr [bp+8]
	add	ax,word ptr [bp-14]
	mov	bx,word ptr [bp-2]
	shl	bx,2
	mov	word ptr DGROUP:_dirProceso[bx+2],dx
	mov	word ptr DGROUP:_dirProceso[bx],ax
   ;	
   ;	        encolarPC2c(indProcesoActual, &(teclado->bloqueados)) ;
   ;	
	mov	ax,word ptr [bp-6]
	add	ax,72
	push	word ptr [bp-4]
	push	ax
	push	word ptr [bp-2]
	call	near ptr _encolarPC2c
	add	sp,6
   ;	
   ;	        bloquearProcesoActual(rec_consola) ;      /* no se retorna de aqui */
   ;	
	push	word ptr DGROUP:_rec_consola
	call	dword ptr DGROUP:_bloquearProcesoActual
	pop	cx
@9@24:
   ;	
   ;	    }
   ;	
   ;	    restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	    return(-1) ;
   ;	
	mov	ax,-1
   ;	
   ;	}
   ;	
	leave	
	ret	
_readConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_aio_readConsola	proc	far
   ;	
   ;	int far aio_readConsola ( int dfs, pointer_t dir, word_t nbytes )
   ;	
	enter	20,0
   ;	
   ;	{
   ;	    pindx_t indProcesoActual ;
   ;	    teclado_t far * teclado ;
   ;	    modoAp_t modoAp ;
   ;	    word_t nbARetornar0, nbARetornar ;
   ;	    int df ;
   ;	    int i = 0 ;
   ;	
	mov	word ptr [bp-16],0
   ;	
   ;	    char car ;
   ;	    word_t con ;
   ;	
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	    setraw_DS() ;           /* establece el DS correspondiente al programa */
   ;	
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
   ;	
   ;	
   ;	    indProcesoActual = *ptrIndProcesoActual ;
   ;	
	les	bx,dword ptr DGROUP:_ptrIndProcesoActual
	mov	ax,word ptr es:[bx]
	mov	word ptr [bp-2],ax
   ;	
   ;	    df = (*ptrTramaProceso)->BX ;
   ;	
	les	bx,dword ptr DGROUP:_ptrTramaProceso
	les	bx,dword ptr es:[bx]
	mov	ax,word ptr es:[bx+12]
	mov	word ptr [bp-14],ax
   ;	
   ;	    con = ptrDescFichero[dfs].menor ;
   ;	
	mov	ax,word ptr [bp+6]
	imul	ax,ax,21
	les	bx,dword ptr DGROUP:_ptrDescFichero
	add	bx,ax
	mov	ax,word ptr es:[bx+13]
	mov	word ptr [bp-20],ax
   ;	
   ;	    teclado = (teclado_t far *)&descConsola[con].teclado ;
   ;	
	mov	ax,word ptr [bp-20]
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola+2
	mov	bx,word ptr DGROUP:_descConsola
	add	bx,ax
	add	bx,8002
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-6],bx
   ;	
   ;	    modoAp = ptrDescProceso[indProcesoActual].tfa[df].modoAp ;
   ;	
	mov	ax,word ptr [bp-2]
	imul	ax,ax,220
	les	bx,dword ptr DGROUP:_ptrDescProceso
	add	bx,ax
	mov	ax,word ptr [bp-14]
	shl	ax,3
	add	bx,ax
	mov	ax,word ptr es:[bx+136]
	mov	word ptr [bp-8],ax
   ;	
   ;	
   ;	    if (nbytes <= teclado->ncar)
   ;	
	les	bx,dword ptr [bp-6]
	mov	ax,word ptr es:[bx+342]
	cmp	ax,word ptr [bp+12]
	jb	short @10@9
   ;	
   ;	        nbARetornar = nbytes ;
   ;	
	mov	ax,word ptr [bp+12]
	mov	word ptr [bp-12],ax
	jmp	short @10@10
@10@9:
   ;	
   ;	    else
   ;	        nbARetornar = teclado->ncar ;
   ;	
	les	bx,dword ptr [bp-6]
	mov	ax,word ptr es:[bx+342]
	mov	word ptr [bp-12],ax
@10@10:
   ;	
   ;	    nbARetornar0 = nbARetornar ;
   ;	
	mov	ax,word ptr [bp-12]
	mov	word ptr [bp-10],ax
	jmp	short @10@15
@10@11:
   ;	
   ;	    while (nbARetornar > 0)
   ;	    {
   ;	        car = sacar(teclado) ;
   ;	
	push	word ptr [bp-4]
	push	word ptr [bp-6]
	call	near ptr _sacar
	add	sp,4
	mov	byte ptr [bp-17],al
   ;	
   ;	        if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
   ;	
	cmp	byte ptr [bp-17],13
	jne	short @10@14
	test	word ptr [bp-8],16384
	je	short @10@14
	mov	byte ptr [bp-17],10
@10@14:
   ;	
   ;	        dir[i++] = car ;
   ;	
	les	bx,dword ptr [bp+8]
	add	bx,word ptr [bp-16]
	mov	al,byte ptr [bp-17]
	mov	byte ptr es:[bx],al
	inc	word ptr [bp-16]
   ;	
   ;	        nbARetornar-- ;
   ;	
	dec	word ptr [bp-12]
@10@15:
	cmp	word ptr [bp-12],0
	ja	short @10@11
   ;	
   ;	    }
   ;	
   ;	    restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	    return(nbARetornar0) ;
   ;	
	mov	ax,word ptr [bp-10]
   ;	
   ;	}
   ;	
	leave	
	ret	
_aio_readConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_writeConsola	proc	far
   ;	
   ;	int far writeConsola ( int dfs, pointer_t dir, word_t nbytes )
   ;	
	enter	6,0
   ;	
   ;	{
   ;	    int i ;
   ;	    char car ;
   ;	    word_t con ;
   ;	
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	    setraw_DS() ;           /* establece el DS correspondiente al programa */
   ;	
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
   ;	
   ;	
   ;	    con = ptrDescFichero[dfs].menor ;
   ;	
	mov	ax,word ptr [bp+6]
	imul	ax,ax,21
	les	bx,dword ptr DGROUP:_ptrDescFichero
	add	bx,ax
	mov	ax,word ptr es:[bx+13]
	mov	word ptr [bp-6],ax
   ;	
   ;	    for ( i = 0 ; i < nbytes ; i++ )
   ;	
	mov	word ptr [bp-2],0
	jmp	short @11@9
@11@7:
   ;	
   ;	    {
   ;	        car = dir[i] ;
   ;	
	les	bx,dword ptr [bp+8]
	add	bx,word ptr [bp-2]
	mov	al,byte ptr es:[bx]
	mov	byte ptr [bp-3],al
   ;	
   ;	        printCarConsola(con, car) ;
   ;	
	mov	al,byte ptr [bp-3]
	push	ax
	mov	al,byte ptr [bp-6]
	push	ax
	call	near ptr _printCarConsola
	add	sp,4
	inc	word ptr [bp-2]
@11@9:
	mov	ax,word ptr [bp-2]
	cmp	ax,word ptr [bp+12]
	jb	short @11@7
   ;	
   ;	    }
   ;	    if (con == consolaDeSuperficie)
   ;	
	mov	ax,word ptr [bp-6]
	cmp	ax,word ptr DGROUP:_consolaDeSuperficie
	jne	short @11@13
   ;	
   ;	        if (cursorC < 80)                      /* ptrBiosArea->VIDEO-width */
   ;	
	cmp	byte ptr DGROUP:_cursorC,80
	jae	short @11@13
   ;	
   ;	            goToXYHw(cursorF, cursorC) ;
   ;	
	mov	al,byte ptr DGROUP:_cursorC
	push	ax
	mov	al,byte ptr DGROUP:_cursorF
	push	ax
	call	near ptr _goToXYHw
	add	sp,4
@11@13:
   ;	
   ;	
   ;	    restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	    return(nbytes) ;
   ;	
	mov	ax,word ptr [bp+12]
   ;	
   ;	}
   ;	
	leave	
	ret	
_writeConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_aio_writeConsola	proc	far
   ;	
   ;	int far aio_writeConsola ( int dfs, pointer_t dir, word_t nbytes )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    return(writeConsola(dfs, dir, nbytes)) ;
   ;	
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	push	cs
	call	near ptr _writeConsola
	add	sp,8
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_aio_writeConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_lseekConsola	proc	far
   ;	
   ;	long int far lseekConsola ( int dfs, long int pos, word_t whence )
   ;	
	enter	10,0
   ;	
   ;	{
   ;	    /* posicionar el cursor */
   ;	
   ;	    word_t con ;
   ;	    long int posActual ;
   ;	    long int res ;
   ;	
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	    setraw_DS() ;           /* establece el DS correspondiente al programa */
   ;	
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
   ;	
   ;	
   ;	    con = ptrDescFichero[dfs].menor ;
   ;	
	mov	ax,word ptr [bp+6]
	imul	ax,ax,21
	les	bx,dword ptr DGROUP:_ptrDescFichero
	add	bx,ax
	mov	ax,word ptr es:[bx+13]
	mov	word ptr [bp-2],ax
   ;	
   ;	
   ;	    posActual = cursorF*80+cursorC ;
   ;	
	mov	al,byte ptr DGROUP:_cursorF
	mov	ah,0
	imul	ax,ax,80
	mov	dl,byte ptr DGROUP:_cursorC
	mov	dh,0
	add	ax,dx
	cwd	
	mov	word ptr [bp-4],dx
	mov	word ptr [bp-6],ax
   ;	
   ;	
   ;	    if (whence == SEEK_CUR)                                    /* SEEK_CUR */
   ;	
	cmp	word ptr [bp+12],1
	jne	short @13@8
   ;	
   ;	        pos = posActual + pos ;
   ;	
	mov	dx,word ptr [bp-4]
	mov	ax,word ptr [bp-6]
	add	ax,word ptr [bp+8]
	adc	dx,word ptr [bp+10]
	mov	word ptr [bp+10],dx
	mov	word ptr [bp+8],ax
@13@8:
   ;	
   ;	
   ;	    if (whence != SEEK_END) {                        /* SEEK_SET, SEEK_CUR */
   ;	
	cmp	word ptr [bp+12],2
	je	short @13@13
   ;	
   ;	        if (goToXYConsola(con, pos/80, pos%80) == 0)
   ;	
	push	0
	push	80
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	call	near ptr N_LMOD@
	push	ax
	push	0
	push	80
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	call	near ptr N_LDIV@
	push	ax
	mov	al,byte ptr [bp-2]
	push	ax
	call	near ptr _goToXYConsola
	add	sp,6
	or	ax,ax
	jne	short @13@11
   ;	
   ;	            res = pos ;
   ;	
	mov	dx,word ptr [bp+10]
	mov	ax,word ptr [bp+8]
	mov	word ptr [bp-8],dx
	mov	word ptr [bp-10],ax
	jmp	short @13@12
@13@11:
   ;	
   ;	        else
   ;	            res = -1 ;
   ;	
	mov	word ptr [bp-8],-1
	mov	word ptr [bp-10],-1
@13@12:
   ;	
   ;	    }
   ;	
	jmp	short @13@14
@13@13:
   ;	
   ;	    else                                                       /* SEEK_END */
   ;	        res = posActual ;
   ;	
	mov	dx,word ptr [bp-4]
	mov	ax,word ptr [bp-6]
	mov	word ptr [bp-8],dx
	mov	word ptr [bp-10],ax
@13@14:
   ;	
   ;	
   ;	    restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	    return(res) ;
   ;	
	mov	dx,word ptr [bp-8]
	mov	ax,word ptr [bp-10]
   ;	
   ;	}
   ;	
	leave	
	ret	
_lseekConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_fcntlConsola	proc	far
   ;	
   ;	int far fcntlConsola ( int dfs, word_t cmd, word_t arg )
   ;	
   ;	
   ;	{
   ;	    return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	ret	
_fcntlConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_ioctlConsola	proc	far
   ;	
   ;	int far ioctlConsola ( int dfs, word_t request, word_t arg )
   ;	
	enter	14,0
   ;	
   ;	{
   ;	    int res ;
   ;	    word_t nuevaConsola ;
   ;	    byte_t con ;
   ;	    byte_t cursorFAux ;
   ;	    pantalla_t far * pantallaAux ;
   ;	    
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	    setraw_DS() ;           /* establece el DS correspondiente al programa */
   ;	
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
   ;	
   ;	
   ;	    res = 0 ;
   ;	
	mov	word ptr [bp-2],0
   ;	
   ;	    if (request == 0x0001)      /* cambiar la consola actual a la indicada */
   ;	
	cmp	word ptr [bp+8],1
	jne	short @15@12
   ;	
   ;	    {
   ;	        nuevaConsola = arg ;
   ;	
	mov	ax,word ptr [bp+10]
	mov	word ptr [bp-4],ax
   ;	
   ;	        if (nuevaConsola > maxConsolas) res = -1 ;
   ;	
	mov	al,byte ptr DGROUP:_maxConsolas
	mov	ah,0
	cmp	ax,word ptr [bp-4]
	jae	short @15@9
	mov	word ptr [bp-2],-1
	jmp	short @15@11
@15@9:
   ;	
   ;	        else if (nuevaConsola != consolaDeSuperficie)
   ;	
	mov	ax,word ptr [bp-4]
	cmp	ax,word ptr DGROUP:_consolaDeSuperficie
	je	short @15@11
   ;	
   ;	        {
   ;	            salvarConsolaDeSuperficie() ;
   ;	
	call	near ptr _salvarConsolaDeSuperficie
   ;	
   ;	            consolaDeSuperficie = nuevaConsola ;
   ;	
	mov	ax,word ptr [bp-4]
	mov	word ptr DGROUP:_consolaDeSuperficie,ax
   ;	
   ;	            establecerConsolaDeSuperficie() ;
   ;	
	call	near ptr _establecerConsolaDeSuperficie
@15@11:
   ;	
   ;	        }
   ;	    }
   ;	
	jmp	@15@32
@15@12:
   ;	
   ;	    else if (request == 0x0002)               /* obtener la consola actual */
   ;	
	cmp	word ptr [bp+8],2
	jne	short @15@14
   ;	
   ;	    {
   ;	        res = consolaDeSuperficie ;
   ;	
	mov	ax,word ptr DGROUP:_consolaDeSuperficie
	mov	word ptr [bp-2],ax
   ;	
   ;	//      printStrBIOS("\n res = ") ;
   ;	//      printDecBIOS(res, 1) ;
   ;	    }
   ;	
	jmp	@15@32
@15@14:
   ;	
   ;	    else if (request == 0x0003) /* cambiar numero de lineas de la pantalla */
   ;	
	cmp	word ptr [bp+8],3
	je	@@1
	jmp	@15@32
@@1:
   ;	
   ;	    {
   ;	        int numFilasAnt = maxFilasAct ;
   ;	
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	mov	word ptr [bp-12],ax
   ;	
   ;	        if (arg <= 0) res = numFilasAnt ;       /* numero de lineas actual */
   ;	
	cmp	word ptr [bp+10],0
	ja	short @15@17
	mov	ax,word ptr [bp-12]
	mov	word ptr [bp-2],ax
	jmp	@15@32
@15@17:
   ;	
   ;	        else if ((arg != 25) && (arg != 28) && (arg != 50)) res = -1 ;
   ;	
	cmp	word ptr [bp+10],25
	je	short @15@21
	cmp	word ptr [bp+10],28
	je	short @15@21
	cmp	word ptr [bp+10],50
	je	short @15@21
	mov	word ptr [bp-2],-1
	jmp	@15@32
@15@21:
   ;	
   ;	        else if (arg == numFilasAnt) res = 0 ;
   ;	
	mov	ax,word ptr [bp+10]
	cmp	ax,word ptr [bp-12]
	jne	short @15@23
	mov	word ptr [bp-2],0
	jmp	@15@32
@15@23:
   ;	
   ;	        else
   ;	        {
   ;				int numFilas = arg ;
   ;	
	mov	ax,word ptr [bp+10]
	mov	word ptr [bp-14],ax
   ;	
   ;	            ptrBiosArea->VIDEO_lastrow = arg - 1 ;
   ;	
	mov	al,byte ptr [bp+10]
	dec	al
	xor	dx,dx
	mov	es,dx
	mov	byte ptr es:[1156],al
   ;	
   ;	            if (cursorF >= numFilas)
   ;	
	mov	al,byte ptr DGROUP:_cursorF
	mov	ah,0
	cmp	ax,word ptr [bp-14]
	jl	short @15@25
   ;	
   ;	            {
   ;	//              copiarPantalla((pantalla_t far *)&ptrPant->t[maxF-numFilas+1][0], ptrPant, numFilas) ;
   ;	                copiarCPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
   ;	
	push	word ptr [bp-14]
	push	word ptr DGROUP:_ptrPant+2
	push	word ptr DGROUP:_ptrPant
	mov	al,byte ptr DGROUP:_cursorF
	mov	ah,0
	sub	ax,word ptr [bp-14]
	inc	ax
	imul	ax,ax,160
	mov	dx,word ptr DGROUP:_ptrPant
	add	dx,ax
	push	word ptr DGROUP:_ptrPant+2
	push	dx
	call	near ptr _copiarCPantalla
	add	sp,10
   ;	
   ;	                borrarCPantalla((pantalla_t far *)&ptrPant->t[numFilas][0], numFilasAnt-numFilas) ;
   ;	
	mov	ax,word ptr [bp-12]
	sub	ax,word ptr [bp-14]
	push	ax
	mov	ax,word ptr [bp-14]
	imul	ax,ax,160
	mov	dx,word ptr DGROUP:_ptrPant
	add	dx,ax
	push	word ptr DGROUP:_ptrPant+2
	push	dx
	call	near ptr _borrarCPantalla
	add	sp,6
   ;	
   ;	                cursorF = numFilas-1 ;
   ;	
	mov	al,byte ptr [bp-14]
	add	al,255
	mov	byte ptr DGROUP:_cursorF,al
@15@25:
   ;	
   ;	            }
   ;	            salvarConsolaDeSuperficie() ;
   ;	
	call	near ptr _salvarConsolaDeSuperficie
   ;	
   ;	            redimensionar(arg, 80) ;
   ;	
	push	80
	mov	al,byte ptr [bp+10]
	push	ax
	call	near ptr _redimensionar
	add	sp,4
   ;	
   ;	            establecerConsolaDeSuperficie() ;
   ;	
	call	near ptr _establecerConsolaDeSuperficie
   ;	
   ;	            res = maxFilasAct ;
   ;	
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	mov	word ptr [bp-2],ax
   ;	
   ;	            if (maxFilasAct == numFilas)
   ;	
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	cmp	ax,word ptr [bp-14]
	je	@@2
	jmp	@15@32
@@2:
   ;	
   ;	            {
   ;	                for ( con = 0 ; con < maxConsolas ; con ++ )
   ;	
	mov	byte ptr [bp-5],0
	jmp	@15@31
@15@27:
   ;	
   ;	                {
   ;	                    if (con != consolaDeSuperficie)
   ;	
	mov	al,byte ptr [bp-5]
	mov	ah,0
	cmp	ax,word ptr DGROUP:_consolaDeSuperficie
	jne	@@3
	jmp	@15@30
@@3:
   ;	
   ;	                    {
   ;	                        cursorFAux = descConsola[con].F ;
   ;	
	mov	al,byte ptr [bp-5]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr es:[bx+8000]
	mov	byte ptr [bp-6],al
   ;	
   ;	                        if (cursorFAux >= numFilas)
   ;	
	mov	al,byte ptr [bp-6]
	mov	ah,0
	cmp	ax,word ptr [bp-14]
	jl	short @15@30
   ;	
   ;	                        {
   ;	                            pantallaAux = (pantalla_t far *)&descConsola[con].pantalla ;
   ;	
	mov	al,byte ptr [bp-5]
	mov	ah,0
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola+2
	mov	bx,word ptr DGROUP:_descConsola
	add	bx,ax
	mov	word ptr [bp-8],dx
	mov	word ptr [bp-10],bx
   ;	
   ;	//                          copiarCPantalla((pantalla_t far *)&pantallaAux->t[maxF-numFilas+1][0], pantallaAux, numFilas) ;
   ;	                            copiarCPantalla((pantalla_t far *)&pantallaAux->t[cursorFAux-numFilas+1][0], pantallaAux, numFilas) 
   ;	
	push	word ptr [bp-14]
	push	word ptr [bp-8]
	push	word ptr [bp-10]
	mov	al,byte ptr [bp-6]
	mov	ah,0
	sub	ax,word ptr [bp-14]
	inc	ax
	imul	ax,ax,160
	mov	dx,word ptr [bp-10]
	add	dx,ax
	push	word ptr [bp-8]
	push	dx
	call	near ptr _copiarCPantalla
	add	sp,10
   ;	
   ;	                            borrarCPantalla((pantalla_t far *)&pantallaAux->t[numFilas][0], numFilasAnt-numFilas) ;
   ;	
	mov	ax,word ptr [bp-12]
	sub	ax,word ptr [bp-14]
	push	ax
	mov	ax,word ptr [bp-14]
	imul	ax,ax,160
	mov	dx,word ptr [bp-10]
	add	dx,ax
	push	word ptr [bp-8]
	push	dx
	call	near ptr _borrarCPantalla
	add	sp,6
   ;	
   ;	                            descConsola[con].F = numFilas-1 ;
   ;	
	mov	al,byte ptr [bp-5]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr [bp-14]
	add	al,255
	mov	byte ptr es:[bx+8000],al
@15@30:
	inc	byte ptr [bp-5]
@15@31:
	mov	al,byte ptr [bp-5]
	cmp	al,byte ptr DGROUP:_maxConsolas
	jae	@@4
	jmp	@15@27
@@4:
@15@32:
   ;	
   ;	                        }
   ;	                    }
   ;	                }
   ;	            }
   ;	        }
   ;	    }
   ;	    restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	    return(res) ;
   ;	
	mov	ax,word ptr [bp-2]
   ;	
   ;	}
   ;	
	leave	
	ret	
_ioctlConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_eliminarConsola	proc	far
   ;	
   ;	int far eliminarConsola ( pindx_t pindx )
   ;	
	enter	6,0
   ;	
   ;	{
   ;	    teclado_t far * teclado ;
   ;	    int i ;
   ;	
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	    setraw_DS() ;           /* establece el DS correspondiente al programa */
   ;	
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
   ;	
   ;	
   ;	    for ( i = 0 ; i < maxConsolas ; i++ )
   ;	
	mov	word ptr [bp-6],0
	jmp	short @16@10
@16@7:
   ;	
   ;	    {
   ;	        teclado = (teclado_t far *)&descConsola[consolaDeSuperficie].teclado ;
   ;	
	mov	ax,word ptr DGROUP:_consolaDeSuperficie
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola+2
	mov	bx,word ptr DGROUP:_descConsola
	add	bx,ax
	add	bx,8002
	mov	word ptr [bp-2],dx
	mov	word ptr [bp-4],bx
   ;	
   ;	        if estaPC2c(pindx, &teclado->bloqueados)
   ;	
	les	bx,dword ptr [bp-4]
	les	bx,dword ptr es:[bx+78]
	mov	ax,word ptr [bp+6]
	shl	ax,2
	add	bx,ax
	cmp	word ptr es:[bx],-1
	je	short @16@9
   ;	
   ;	        {
   ;	            eliminarPC2c(pindx, &teclado->bloqueados) ;
   ;	
	mov	ax,word ptr [bp-4]
	add	ax,72
	push	word ptr [bp-2]
	push	ax
	push	word ptr [bp+6]
	call	near ptr _eliminarPC2c
	add	sp,6
   ;	
   ;	            break ;
   ;	
	jmp	short @16@11
@16@9:
	inc	word ptr [bp-6]
@16@10:
	mov	al,byte ptr DGROUP:_maxConsolas
	mov	ah,0
	cmp	ax,word ptr [bp-6]
	jg	short @16@7
@16@11:
   ;	
   ;	        }
   ;	    }
   ;	
   ;	    restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	    return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	leave	
	ret	
_eliminarConsola	endp
_TEXT	ends
_DATA	segment word public 'DATA'
_keyStroke	label	byte
	db	0
_scanCode	label	byte
	db	0
_scanCodeAnt	label	byte
	db	0
_AltPulsada	label	byte
	db	0
_CtrlPulsada	label	byte
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_isr_consola	proc	far
   ;	
   ;	void far isr_consola ( void )
   ;	
	enter	36,0
	push	si
   ;	
   ;	{
   ;	    word_t w ;
   ;	    char car ;
   ;	    word_t nbytes ;
   ;	    pointer_t dir ;
   ;	    int df ;
   ;	    pindx_t indProcesoActual ;
   ;	    pindx_t pindx ;
   ;	    modoAp_t modoAp ;
   ;	    bool_t extendido ;
   ;	    teclado_t far * teclado ;
   ;	    byte_t con ;
   ;	    byte_t nuevaConsola ;
   ;	    word_t numFilas ;
   ;	    word_t numFilasAnt ;
   ;	    byte_t cursorFAux ;
   ;	    pantalla_t far * pantallaAux ;
   ;	
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	    setraw_DS() ;           /* establece el DS correspondiente al programa */
   ;	
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
   ;	
   ;	
   ;	    scanCodeAnt = scanCode ;
   ;	
	mov	al,byte ptr DGROUP:_scanCode
	mov	byte ptr DGROUP:_scanCodeAnt,al
   ;	
   ;	    asm in al,60h       /* ¿solo debe leerse una vez el registro de datos? */
   ;	
 	in	 al,60h       
   ;	
   ;	    asm mov scanCode,al
   ;	
 	mov	 DGROUP:_scanCode,al
   ;	
   ;	
   ;	    ptrVIOrg[nVIntConsola]() ;  /* llamamos a la antigua rti (pushf + int) */
   ;	
	les	bx,dword ptr DGROUP:_ptrVIOrg
	pushf	
	call	dword ptr es:[bx+36]
   ;	
   ;	
   ;	    nuevaConsola = 0xFF ;
   ;	
	mov	byte ptr [bp-26],255
   ;	
   ;	    numFilasAnt = maxFilasAct ;
   ;	
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	mov	word ptr [bp-30],ax
   ;	
   ;	
   ;	#if (FALSE)                               /* trazas por si hay que depurar */
   ;	    printCarBIOS('<') ;
   ;	    printHexBIOS(scanCodeAnt, 2) ;
   ;	    printHexBIOS(scanCode, 2) ;
   ;	    printCarBIOS('>') ;
   ;	#endif
   ;	
   ;	    if (scanCodeAnt == CS_Alt) AltPulsada = TRUE ;
   ;	
	cmp	byte ptr DGROUP:_scanCodeAnt,56
	jne	short @17@11
	mov	byte ptr DGROUP:_AltPulsada,1
	jmp	short @17@13
@17@11:
   ;	
   ;	    else if (scanCodeAnt == (CS_Alt | 0x80)) AltPulsada = FALSE ;
   ;	
	cmp	byte ptr DGROUP:_scanCodeAnt,184
	jne	short @17@13
	mov	byte ptr DGROUP:_AltPulsada,0
@17@13:
   ;	
   ;	
   ;	    if (scanCodeAnt == CS_Ctrl) CtrlPulsada = TRUE ;
   ;	
	cmp	byte ptr DGROUP:_scanCodeAnt,29
	jne	short @17@15
	mov	byte ptr DGROUP:_CtrlPulsada,1
	jmp	short @17@17
@17@15:
   ;	
   ;	    else if (scanCodeAnt == (CS_Ctrl | 0x80)) CtrlPulsada = FALSE ;
   ;	
	cmp	byte ptr DGROUP:_scanCodeAnt,157
	jne	short @17@17
	mov	byte ptr DGROUP:_CtrlPulsada,0
@17@17:
   ;	
   ;	
   ;	    if ((CtrlPulsada) && (scanCode == 0x26))                     /* Ctrl+L */
   ;	
	cmp	byte ptr DGROUP:_CtrlPulsada,0
	je	short @17@20
	cmp	byte ptr DGROUP:_scanCode,38
	jne	short @17@20
   ;	
   ;	        cambiarTeclaListaBDA((teclaListaBDA() & 0xFF00) | '\f') ;
   ;	
	call	near ptr _teclaListaBDA
	and	ax,-256
	or	ax,12
	push	ax
	call	near ptr _cambiarTeclaListaBDA
	pop	cx
@17@20:
   ;	
   ;	
   ;	    if (AltPulsada)
   ;	
	cmp	byte ptr DGROUP:_AltPulsada,0
	jne	@@5
	jmp	@17@62
@@5:
   ;	
   ;	    {
   ;	
   ;	        switch (scanCode)
   ;	
	mov	al,byte ptr DGROUP:_scanCode
	mov	ah,0
	sub	ax,41
	mov	bx,ax
	cmp	bx,27
	ja	short @17@28
	add	bx,bx
	jmp	word ptr cs:@17@C212[bx]
@17@24:
   ;	
   ;	        {
   ;	
   ;	        case CS_ES_BSlash :
   ;	            nuevaConsola = 0 ;
   ;	
	mov	byte ptr [bp-26],0
   ;	
   ;	            break ;
   ;	
	jmp	short @17@28
@17@25:
   ;	
   ;	
   ;	        case CS_F1     : ;
   ;	        case CS_F2     : ;
   ;	        case CS_F3     : ;
   ;	        case CS_F4     : ;
   ;	        case CS_F5     : ;
   ;	        case CS_F6     : ;
   ;	        case CS_F7     : ;
   ;	        case CS_F8     : ;
   ;	        case CS_F9     : ;
   ;	        case CS_F10    :
   ;	            con = scanCode - CS_F1 + 1 ;
   ;	
	mov	al,byte ptr DGROUP:_scanCode
	add	al,198
	mov	byte ptr [bp-25],al
   ;	
   ;	            if (con < maxConsolas)
   ;	
	mov	al,byte ptr [bp-25]
	cmp	al,byte ptr DGROUP:_maxConsolas
	jae	short @17@27
   ;	
   ;	                nuevaConsola = con ;
   ;	
	mov	al,byte ptr [bp-25]
	mov	byte ptr [bp-26],al
@17@27:
   ;	
   ;	            break ;
   ;	
	jmp	short @17@28
@17@28:
   ;	
   ;	        default        :
   ;	            ;
   ;	        }
   ;	
   ;	        if (nuevaConsola != 0xFF)
   ;	
	cmp	byte ptr [bp-26],255
	je	short @17@32
   ;	
   ;	        {
   ;	            if (nuevaConsola != consolaDeSuperficie)
   ;	
	mov	al,byte ptr [bp-26]
	mov	ah,0
	cmp	ax,word ptr DGROUP:_consolaDeSuperficie
	je	short @17@31
   ;	
   ;	            {
   ;	                salvarConsolaDeSuperficie() ;
   ;	
	call	near ptr _salvarConsolaDeSuperficie
   ;	
   ;	                consolaDeSuperficie = nuevaConsola ;
   ;	
	mov	al,byte ptr [bp-26]
	mov	ah,0
	mov	word ptr DGROUP:_consolaDeSuperficie,ax
   ;	
   ;	                establecerConsolaDeSuperficie() ;
   ;	
	call	near ptr _establecerConsolaDeSuperficie
@17@31:
   ;	
   ;	            }
   ;	        }
   ;	
	jmp	@17@59
@17@32:
   ;	
   ;	        else if (redimensionable)
   ;	
	cmp	word ptr DGROUP:_redimensionable,0
	jne	@@6
	jmp	@17@59
@@6:
   ;	
   ;	        {
   ;	            switch (scanCode)
   ;	
	mov	al,byte ptr DGROUP:_scanCode
	mov	ah,0
	cmp	ax,27
	je	short @17@37
	cmp	ax,53
	je	short @17@37
	jmp	@17@59
@17@37:
   ;	
   ;	            {
   ;	            case 0x1B : ;                                          /* Alt+ */
   ;	            case 0x35 : ;                                          /* Alt- */
   ;	                if (scanCode == 0x1B)                              /* Alt+ */
   ;	
	cmp	byte ptr DGROUP:_scanCode,27
	jne	short @17@44
   ;	
   ;	                {
   ;	                    if (numFilasAnt >= 50) numFilas = 25 ;
   ;	
	cmp	word ptr [bp-30],50
	jb	short @17@40
	mov	word ptr [bp-28],25
	jmp	short @17@43
@17@40:
   ;	
   ;	                    else if (numFilasAnt >= 28) numFilas = 50 ;
   ;	
	cmp	word ptr [bp-30],28
	jb	short @17@42
	mov	word ptr [bp-28],50
	jmp	short @17@43
@17@42:
   ;	
   ;	                    else numFilas = 28 ;
   ;	
	mov	word ptr [bp-28],28
@17@43:
   ;	
   ;	                }
   ;	
	jmp	short @17@49
@17@44:
   ;	
   ;	                else                                               /* Alt- */
   ;	                {
   ;	                    if (numFilasAnt >= 50) numFilas = 28 ;
   ;	
	cmp	word ptr [bp-30],50
	jb	short @17@46
	mov	word ptr [bp-28],28
	jmp	short @17@49
@17@46:
   ;	
   ;	                    else if (numFilasAnt >= 28) numFilas = 25 ;
   ;	
	cmp	word ptr [bp-30],28
	jb	short @17@48
	mov	word ptr [bp-28],25
	jmp	short @17@49
@17@48:
   ;	
   ;	                    else numFilas = 50 ;
   ;	
	mov	word ptr [bp-28],50
@17@49:
   ;	
   ;	                }
   ;	////            if (numFilas != numFilasAnt)
   ;	////            {
   ;	                    if (cursorF >= numFilas)
   ;	
	mov	al,byte ptr DGROUP:_cursorF
	mov	ah,0
	cmp	ax,word ptr [bp-28]
	jb	short @17@51
   ;	
   ;	                    {
   ;	                        copiarCPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
   ;	
	push	word ptr [bp-28]
	push	word ptr DGROUP:_ptrPant+2
	push	word ptr DGROUP:_ptrPant
	mov	al,byte ptr DGROUP:_cursorF
	mov	ah,0
	sub	ax,word ptr [bp-28]
	inc	ax
	imul	ax,ax,160
	mov	dx,word ptr DGROUP:_ptrPant
	add	dx,ax
	push	word ptr DGROUP:_ptrPant+2
	push	dx
	call	near ptr _copiarCPantalla
	add	sp,10
   ;	
   ;	                        borrarCPantalla((pantalla_t far *)&ptrPant->t[numFilas][0], numFilasAnt-numFilas) ;
   ;	
	mov	ax,word ptr [bp-30]
	sub	ax,word ptr [bp-28]
	push	ax
	mov	ax,word ptr [bp-28]
	imul	ax,ax,160
	mov	dx,word ptr DGROUP:_ptrPant
	add	dx,ax
	push	word ptr DGROUP:_ptrPant+2
	push	dx
	call	near ptr _borrarCPantalla
	add	sp,6
   ;	
   ;	//                      copiarPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
   ;	                        cursorF = numFilas-1 ;
   ;	
	mov	al,byte ptr [bp-28]
	dec	al
	mov	byte ptr DGROUP:_cursorF,al
@17@51:
   ;	
   ;	                    }
   ;	                    salvarConsolaDeSuperficie() ;
   ;	
	call	near ptr _salvarConsolaDeSuperficie
   ;	
   ;	                    redimensionar(numFilas, 80) ;
   ;	
	push	80
	mov	al,byte ptr [bp-28]
	push	ax
	call	near ptr _redimensionar
	add	sp,4
   ;	
   ;	#define MSDOS_Player 1
   ;	#ifdef MSDOS_Player
   ;	                    goToXYHw(24, 79) ;
   ;	
	push	79
	push	24
	call	near ptr _goToXYHw
	add	sp,4
   ;	
   ;	                    goToXYHw(cursorF, cursorC) ;   /* necesario para MSDOS Player (Takeda) ??? */
   ;	
	mov	al,byte ptr DGROUP:_cursorC
	push	ax
	mov	al,byte ptr DGROUP:_cursorF
	push	ax
	call	near ptr _goToXYHw
	add	sp,4
   ;	
   ;	#endif
   ;	                    if (numFilas == ptrBiosArea->VIDEO_lastrow + 1)
   ;	
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	cmp	ax,word ptr [bp-28]
	je	@@7
	jmp	@17@58
@@7:
   ;	
   ;	                    {
   ;	                        establecerConsolaDeSuperficie() ;
   ;	
	call	near ptr _establecerConsolaDeSuperficie
   ;	
   ;	                        for ( con = 0 ; con < maxConsolas ; con ++ )
   ;	
	mov	byte ptr [bp-25],0
	jmp	@17@57
@17@53:
   ;	
   ;	                        {
   ;	                            if (con != consolaDeSuperficie)
   ;	
	mov	al,byte ptr [bp-25]
	mov	ah,0
	cmp	ax,word ptr DGROUP:_consolaDeSuperficie
	jne	@@8
	jmp	@17@56
@@8:
   ;	
   ;	                            {
   ;	                                cursorFAux = descConsola[con].F ;
   ;	
	mov	al,byte ptr [bp-25]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr es:[bx+8000]
	mov	byte ptr [bp-31],al
   ;	
   ;	                                if (cursorFAux >= numFilas)
   ;	
	mov	al,byte ptr [bp-31]
	mov	ah,0
	cmp	ax,word ptr [bp-28]
	jb	short @17@56
   ;	
   ;	                                {
   ;	                                    pantallaAux = (pantalla_t far *)&descConsola[con].pantalla ;
   ;	
	mov	al,byte ptr [bp-25]
	mov	ah,0
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola+2
	mov	bx,word ptr DGROUP:_descConsola
	add	bx,ax
	mov	word ptr [bp-34],dx
	mov	word ptr [bp-36],bx
   ;	
   ;	                                    copiarCPantalla((pantalla_t far *)&pantallaAux->t[cursorFAux-numFilas+1][0], pantallaAux, nu
   ;	
	push	word ptr [bp-28]
	push	word ptr [bp-34]
	push	word ptr [bp-36]
	mov	al,byte ptr [bp-31]
	mov	ah,0
	sub	ax,word ptr [bp-28]
	inc	ax
	imul	ax,ax,160
	mov	dx,word ptr [bp-36]
	add	dx,ax
	push	word ptr [bp-34]
	push	dx
	call	near ptr _copiarCPantalla
	add	sp,10
   ;	
   ;	                                    borrarCPantalla((pantalla_t far *)&pantallaAux->t[numFilas][0], numFilasAnt-numFilas) ;
   ;	
	mov	ax,word ptr [bp-30]
	sub	ax,word ptr [bp-28]
	push	ax
	mov	ax,word ptr [bp-28]
	imul	ax,ax,160
	mov	dx,word ptr [bp-36]
	add	dx,ax
	push	word ptr [bp-34]
	push	dx
	call	near ptr _borrarCPantalla
	add	sp,6
   ;	
   ;	                                    descConsola[con].F = numFilas-1 ;
   ;	
	mov	al,byte ptr [bp-25]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	al,byte ptr [bp-28]
	dec	al
	mov	byte ptr es:[bx+8000],al
@17@56:
	inc	byte ptr [bp-25]
@17@57:
	mov	al,byte ptr [bp-25]
	cmp	al,byte ptr DGROUP:_maxConsolas
	jae	@@9
	jmp	@17@53
@@9:
@17@58:
   ;	
   ;	                                }
   ;	                            }
   ;	                        }
   ;	                    }
   ;	////            }
   ;	                break ;
   ;	
	jmp	short @17@59
@17@59:
   ;	
   ;	            default :
   ;	                ;
   ;	            }
   ;	        }
   ;	        w = leerTeclaListaBDA() ;                        /* /PP 2016-10-31 */
   ;	
	call	near ptr _leerTeclaListaBDA
	mov	word ptr [bp-2],ax
   ;	
   ;	        restore_DS0() ;                           /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	        return ;                                         /* /PP 2016-10-31 */
   ;	
	jmp	@17@86
@17@62:
   ;	
   ;	    }
   ;	
   ;	    if (scanCode & 0x80)
   ;	
	test	byte ptr DGROUP:_scanCode,128
	je	short @17@65
   ;	
   ;	    {
   ;	        restore_DS0() ;                           /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	        return ;
   ;	
	jmp	@17@86
@17@65:
   ;	
   ;	    }
   ;	
   ;	#if (!usa_BIOS)
   ;	    w = leerTeclaListaBDA() ;           /* para sacar la palabra del bufer */
   ;	
	call	near ptr _leerTeclaListaBDA
	mov	word ptr [bp-2],ax
   ;	
   ;	//puntoDeParada() ;
   ;	#else
   ;	    w = teclaListaBIOS() ;
   ;	    if (w) leerTeclaBIOS() ;            /* para sacar la palabra del bufer */
   ;	    /* funciona mal en qemu/bochs ¿permite las interrupciones? */
   ;	//  puntoDeParada() ;
   ;	#endif
   ;	
   ;	//  printHexBIOS(w, 4) ; printCarBIOS(' ') ;
   ;	
   ;	    if (w)
   ;	
	cmp	word ptr [bp-2],0
	jne	@@10
	jmp	@17@85
@@10:
   ;	
   ;	    {
   ;	        car = (char)w ;
   ;	
	mov	al,byte ptr [bp-2]
	mov	byte ptr [bp-3],al
   ;	
   ;	//      car = tablaSP[car] ;
   ;	        car = tablaDeConversion[car] ;
   ;	
	mov	al,byte ptr [bp-3]
	cbw	
	mov	bx,ax
	mov	al,byte ptr DGROUP:_tablaSP[bx]
	mov	byte ptr [bp-3],al
   ;	
   ;	
   ;	        if (car == (char)0xE0)
   ;	
	cmp	byte ptr [bp-3],224
	jne	short @17@68
   ;	
   ;	            car = '\0' ;
   ;	
	mov	byte ptr [bp-3],0
@17@68:
   ;	
   ;	        extendido = (car == '\0') ;
   ;	
	cmp	byte ptr [bp-3],0
	jne	short @17@70
	mov	ax,1
	jmp	short @17@71
@17@70:
	xor	ax,ax
@17@71:
	mov	word ptr [bp-20],ax
   ;	
   ;	
   ;	        teclado = (teclado_t far *)&descConsola[consolaDeSuperficie].teclado ;
   ;	
	mov	ax,word ptr DGROUP:_consolaDeSuperficie
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola+2
	mov	bx,word ptr DGROUP:_descConsola
	add	bx,ax
	add	bx,8002
	mov	word ptr [bp-22],dx
	mov	word ptr [bp-24],bx
   ;	
   ;	
   ;	        if (teclado->bloqueados.numElem > 0)
   ;	
	les	bx,dword ptr [bp-24]
	cmp	word ptr es:[bx+72],0
	jg	@@11
	jmp	@17@83
@@11:
   ;	
   ;	        {
   ;	            pindx = teclado->bloqueados.primero ;
   ;	
	les	bx,dword ptr [bp-24]
	mov	ax,word ptr es:[bx+74]
	mov	word ptr [bp-16],ax
   ;	
   ;	            df = ptrDescProceso[pindx].trama->BX ;
   ;	
	mov	ax,word ptr [bp-16]
	imul	ax,ax,220
	les	bx,dword ptr DGROUP:_ptrDescProceso
	add	bx,ax
	les	bx,dword ptr es:[bx+6]
	mov	ax,word ptr es:[bx+12]
	mov	word ptr [bp-12],ax
   ;	
   ;	            modoAp = ptrDescProceso[pindx].tfa[df].modoAp ;
   ;	
	mov	ax,word ptr [bp-16]
	imul	ax,ax,220
	les	bx,dword ptr DGROUP:_ptrDescProceso
	add	bx,ax
	mov	ax,word ptr [bp-12]
	shl	ax,3
	add	bx,ax
	mov	ax,word ptr es:[bx+136]
	mov	word ptr [bp-18],ax
   ;	
   ;	            /*    plot('k', car, contadorTimer0()) ; */
   ;	            nbytes = nbytesProceso[pindx] ;
   ;	
	mov	bx,word ptr [bp-16]
	add	bx,bx
	mov	ax,word ptr DGROUP:_nbytesProceso[bx]
	mov	word ptr [bp-6],ax
   ;	
   ;	            dir = dirProceso[pindx] ;
   ;	
	mov	bx,word ptr [bp-16]
	shl	bx,2
	mov	dx,word ptr DGROUP:_dirProceso[bx+2]
	mov	ax,word ptr DGROUP:_dirProceso[bx]
	mov	word ptr [bp-8],dx
	mov	word ptr [bp-10],ax
   ;	
   ;	            nbytes-- ;
   ;	
	dec	word ptr [bp-6]
   ;	
   ;	            if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
   ;	
	cmp	byte ptr [bp-3],13
	jne	short @17@75
	test	word ptr [bp-18],16384
	je	short @17@75
	mov	byte ptr [bp-3],10
@17@75:
   ;	
   ;	            dir[0] = car ;                                          /* car */
   ;	
	les	bx,dword ptr [bp-10]
	mov	al,byte ptr [bp-3]
	mov	byte ptr es:[bx],al
   ;	
   ;	            dir++ ;
   ;	
	inc	word ptr [bp-10]
   ;	
   ;	            if (extendido)
   ;	
	cmp	word ptr [bp-20],0
	je	short @17@79
   ;	
   ;	            {
   ;	                keyStroke = (byte_t)(w >> 8) ;
   ;	
	mov	ax,word ptr [bp-2]
	shr	ax,8
	mov	byte ptr DGROUP:_keyStroke,al
   ;	
   ;	                car = (char)keyStroke ;
   ;	
	mov	al,byte ptr DGROUP:_keyStroke
	mov	byte ptr [bp-3],al
   ;	
   ;	                if (nbytes > 0)
   ;	
	cmp	word ptr [bp-6],0
	jbe	short @17@78
   ;	
   ;	                {
   ;	                    nbytes-- ;
   ;	
	dec	word ptr [bp-6]
   ;	
   ;	                    dir[1] = car ;                             /* scanCode */
   ;	
	les	bx,dword ptr [bp-10]
	mov	al,byte ptr [bp-3]
	mov	byte ptr es:[bx+1],al
   ;	
   ;	                    dir++ ;
   ;	
	inc	word ptr [bp-10]
   ;	
   ;	                }
   ;	
	jmp	short @17@79
@17@78:
   ;	
   ;	                else
   ;	                    meter(car, teclado) ;
   ;	
	push	word ptr [bp-22]
	push	word ptr [bp-24]
	mov	al,byte ptr [bp-3]
	push	ax
	call	near ptr _meter
	add	sp,6
@17@79:
   ;	
   ;	            }
   ;	            nbytesProceso[pindx] = nbytes ;
   ;	
	mov	bx,word ptr [bp-16]
	add	bx,bx
	mov	ax,word ptr [bp-6]
	mov	word ptr DGROUP:_nbytesProceso[bx],ax
   ;	
   ;	            dirProceso[pindx] = dir ;
   ;	
	mov	bx,word ptr [bp-16]
	shl	bx,2
	mov	dx,word ptr [bp-8]
	mov	ax,word ptr [bp-10]
	mov	word ptr DGROUP:_dirProceso[bx+2],dx
	mov	word ptr DGROUP:_dirProceso[bx],ax
   ;	
   ;	            if (nbytes == 0)
   ;	
	cmp	word ptr [bp-6],0
	je	@@12
	jmp	@17@82
@@12:
   ;	
   ;	            {
   ;	                desencolarPC2c(&teclado->bloqueados) ;
   ;	
	mov	ax,word ptr [bp-24]
	add	ax,72
	push	word ptr [bp-22]
	push	ax
	call	near ptr _desencolarPC2c
	add	sp,4
   ;	
   ;	                ptrDescProceso[pindx].trama->AX =
   ;	
   ;	
   ;	                    ptrDescProceso[pindx].trama->CX ;           /* nbytes0 */
   ;	
	mov	ax,word ptr [bp-16]
	imul	ax,ax,220
	les	bx,dword ptr DGROUP:_ptrDescProceso
	add	bx,ax
	les	bx,dword ptr es:[bx+6]
	mov	ax,word ptr es:[bx+16]
	mov	dx,word ptr [bp-16]
	imul	dx,dx,220
	les	bx,dword ptr DGROUP:_ptrDescProceso
	add	bx,dx
	les	bx,dword ptr es:[bx+6]
	mov	word ptr es:[bx+18],ax
   ;	
   ;	                ptrDescProceso[pindx].tfa[df].pos +=
   ;	
   ;	
   ;	                    ptrDescProceso[pindx].trama->CX ;
   ;	
	mov	ax,word ptr [bp-16]
	imul	ax,ax,220
	les	bx,dword ptr DGROUP:_ptrDescProceso
	add	bx,ax
	les	bx,dword ptr es:[bx+6]
	mov	ax,word ptr es:[bx+16]
	mov	dx,word ptr [bp-16]
	imul	dx,dx,220
	les	bx,dword ptr DGROUP:_ptrDescProceso
	add	bx,dx
	mov	dx,word ptr [bp-12]
	shl	dx,3
	add	bx,dx
	add	word ptr es:[bx+140],ax
	adc	word ptr es:[bx+142],0
   ;	
   ;	                ptrDescProceso[pindx].estado = preparado ;
   ;	
	mov	ax,word ptr [bp-16]
	imul	ax,ax,220
	les	bx,dword ptr DGROUP:_ptrDescProceso
	add	bx,ax
	mov	word ptr es:[bx+2],1
   ;	
   ;	                encolarPC2c(pindx, (ptrC2c_t)&ptrC2cPFR[PUrgentes]) ;
   ;	
	mov	ax,word ptr DGROUP:_ptrC2cPFR
	add	ax,30
	push	word ptr DGROUP:_ptrC2cPFR+2
	push	ax
	push	word ptr [bp-16]
	call	near ptr _encolarPC2c
	add	sp,6
   ;	
   ;	                *ptrActivarAlEpilogo = TRUE ;
   ;	
	les	bx,dword ptr DGROUP:_ptrActivarAlEpilogo
	mov	word ptr es:[bx],1
   ;	
   ;	                indProcesoActual = *ptrIndProcesoActual ;
   ;	
	les	bx,dword ptr DGROUP:_ptrIndProcesoActual
	mov	ax,word ptr es:[bx]
	mov	word ptr [bp-14],ax
   ;	
   ;	                if (indProcesoActual > -1)                  /* nivel 1 o 2 */
   ;	
	cmp	word ptr [bp-14],-1
	jle	short @17@82
   ;	
   ;	                {
   ;	                    ptrDescProceso[indProcesoActual].trama =
   ;	
   ;	
   ;	                        MK_FP(*ptrSS_Proceso, *ptrSP_Proceso) ;
   ;	
	les	bx,dword ptr DGROUP:_ptrSS_Proceso
	mov	ax,word ptr es:[bx]
	xor	dx,dx
	les	bx,dword ptr DGROUP:_ptrSP_Proceso
	or	dx,word ptr es:[bx]
	mov	bx,word ptr [bp-14]
	imul	bx,bx,220
	les	si,dword ptr DGROUP:_ptrDescProceso
	add	si,bx
	mov	word ptr es:[si+8],ax
	mov	word ptr es:[si+6],dx
   ;	
   ;	                    ptrDescProceso[indProcesoActual].estado = preparado ;
   ;	
	mov	ax,word ptr [bp-14]
	imul	ax,ax,220
	les	bx,dword ptr DGROUP:_ptrDescProceso
	add	bx,ax
	mov	word ptr es:[bx+2],1
   ;	
   ;	                    encolarPC2c(indProcesoActual, (ptrC2c_t)&ptrC2cPFR[PPreparados]) ;
   ;	
	mov	ax,word ptr DGROUP:_ptrC2cPFR
	add	ax,20
	push	word ptr DGROUP:_ptrC2cPFR+2
	push	ax
	push	word ptr [bp-14]
	call	near ptr _encolarPC2c
	add	sp,6
   ;	
   ;	                    *ptrIndProcesoActual = -1 ;
   ;	
	les	bx,dword ptr DGROUP:_ptrIndProcesoActual
	mov	word ptr es:[bx],-1
@17@82:
   ;	
   ;	                }
   ;	            }
   ;	        }
   ;	
	jmp	short @17@85
@17@83:
   ;	
   ;	        else
   ;	        {
   ;	            meter(car, teclado) ;                                   /* car */
   ;	
	push	word ptr [bp-22]
	push	word ptr [bp-24]
	mov	al,byte ptr [bp-3]
	push	ax
	call	near ptr _meter
	add	sp,6
   ;	
   ;	            if (extendido)
   ;	
	cmp	word ptr [bp-20],0
	je	short @17@85
   ;	
   ;	            {
   ;	                keyStroke = (byte_t)(w >> 8) ;
   ;	
	mov	ax,word ptr [bp-2]
	shr	ax,8
	mov	byte ptr DGROUP:_keyStroke,al
   ;	
   ;	                car = (char)keyStroke ;                       /* keyStroke */
   ;	
	mov	al,byte ptr DGROUP:_keyStroke
	mov	byte ptr [bp-3],al
   ;	
   ;	                meter(car, teclado) ;
   ;	
	push	word ptr [bp-22]
	push	word ptr [bp-24]
	mov	al,byte ptr [bp-3]
	push	ax
	call	near ptr _meter
	add	sp,6
@17@85:
   ;	
   ;	            }
   ;	        }
   ;	    }
   ;	    /*
   ;	    if (*info.ptrDebugWord > 0)
   ;	    {
   ;	        printStrBIOS("\n debugWord = ") ;
   ;	        printHexBIOS(*info.ptrDebugWord, 4) ;
   ;	        printStrBIOS("\n isr_consola pto 2: ") ;
   ;	        printStrBIOS("\n indProcesoActual = ") ;
   ;	        printIntBIOS(indProcesoActual, 1) ;
   ;	        printStrBIOS(" pindx = ") ;
   ;	        printIntBIOS(pindx, 1) ;
   ;	        *info.ptrDebugWord = *info.ptrDebugWord - 1 ;
   ;	        if (*info.ptrDebugWord == 0) while(TRUE) ;
   ;	    }
   ;	    */
   ;	    restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
@17@86:
   ;	
   ;	}
   ;	
	pop	si
	leave	
	ret	
_isr_consola	endp
@17@C212	label	word
	dw	@17@24
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@28
	dw	@17@25
	dw	@17@25
	dw	@17@25
	dw	@17@25
	dw	@17@25
	dw	@17@25
	dw	@17@25
	dw	@17@25
	dw	@17@25
	dw	@17@25
_TEXT	ends
_DATA	segment word public 'DATA'
_congen	label	byte
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_pCon	proc	near
   ;	
   ;	int pCon ( char car )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    printCarConsola(congen, car) ;
   ;	
	mov	al,byte ptr [bp+4]
	push	ax
	mov	al,byte ptr DGROUP:_congen
	push	ax
	call	near ptr _printCarConsola
	add	sp,4
   ;	
   ;	    return(1) ;
   ;	
	mov	ax,1
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_pCon	endp
	assume	cs:_TEXT,ds:DGROUP
_printLnConsola	proc	near
   ;	
   ;	int printLnConsola ( byte_t con )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    congen = con ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	byte ptr DGROUP:_congen,al
   ;	
   ;	    return(printGenLn(pCon)) ;
   ;	
	push	offset _pCon
	call	near ptr _printGenLn
	pop	cx
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_printLnConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_printStrConsola	proc	near
   ;	
   ;	int printStrConsola ( byte_t con, const char far * str )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    congen = con ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	byte ptr DGROUP:_congen,al
   ;	
   ;	    return(printGenStr(str, pCon)) ;
   ;	
	push	offset _pCon
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr _printGenStr
	add	sp,6
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_printStrConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_printStrHastaConsola	proc	near
   ;	
   ;	int printStrHastaConsola ( byte_t con, const char far * str, word_t n, bool_t lleno )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    congen = con ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	byte ptr DGROUP:_congen,al
   ;	
   ;	    return(printGenStrHasta(str, n, lleno, pCon)) ;
   ;	
	push	offset _pCon
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr _printGenStrHasta
	add	sp,10
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_printStrHastaConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_printDecConsola	proc	near
   ;	
   ;	int printDecConsola ( byte_t con, word_t num, word_t l )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    congen = con ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	byte ptr DGROUP:_congen,al
   ;	
   ;	    return(printGenDec(num, l, pCon)) ;
   ;	
	push	offset _pCon
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr _printGenDec
	add	sp,6
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_printDecConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_printIntConsola	proc	near
   ;	
   ;	int printIntConsola ( byte_t con, int num, word_t l )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    congen = con ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	byte ptr DGROUP:_congen,al
   ;	
   ;	    return(printGenInt(num, l, pCon)) ;
   ;	
	push	offset _pCon
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr _printGenInt
	add	sp,6
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_printIntConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_printLIntConsola	proc	near
   ;	
   ;	int printLIntConsola ( byte_t con, long int num, word_t l )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    congen = con ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	byte ptr DGROUP:_congen,al
   ;	
   ;	    return(printGenLInt(num, l, pCon)) ;
   ;	
	push	offset _pCon
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr _printGenLInt
	add	sp,8
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_printLIntConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_printLDecConsola	proc	near
   ;	
   ;	int printLDecConsola ( byte_t con, dword_t num, word_t l )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    congen = con ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	byte ptr DGROUP:_congen,al
   ;	
   ;	    return(printGenLDec(num, l, pCon)) ;
   ;	
	push	offset _pCon
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr _printGenLDec
	add	sp,8
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_printLDecConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_printHexConsola	proc	near
   ;	
   ;	int printHexConsola ( byte_t con, word_t num, word_t l )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    congen = con ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	byte ptr DGROUP:_congen,al
   ;	
   ;	    return(printGenHex(num, l, pCon)) ;
   ;	
	push	offset _pCon
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr _printGenHex
	add	sp,6
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_printHexConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_printLHexConsola	proc	near
   ;	
   ;	int printLHexConsola ( byte_t con, dword_t num, word_t l )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    congen = con ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	byte ptr DGROUP:_congen,al
   ;	
   ;	    return(printGenLHex(num, l, pCon)) ;
   ;	
	push	offset _pCon
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr _printGenLHex
	add	sp,8
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_printLHexConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_printPtrConsola	proc	near
   ;	
   ;	int printPtrConsola ( byte_t con, pointer_t ptr )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    congen = con ;
   ;	
	mov	al,byte ptr [bp+4]
	mov	byte ptr DGROUP:_congen,al
   ;	
   ;	    return(printGenPtr(ptr, pCon)) ;
   ;	
	push	offset _pCon
	push	word ptr [bp+8]
	push	word ptr [bp+6]
	call	near ptr _printGenPtr
	add	sp,6
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_printPtrConsola	endp
_TEXT	ends
_DATA	segment word public 'DATA'
_strCbBlancos	label	byte
	db	0
_strCbCON	label	byte
	db	67
	db	79
	db	78
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_cbForTimer	proc	far
   ;	
   ;	int far cbForTimer ( void far * arg )                         /* call back */
   ;	
	enter	4,0
   ;	
   ;	{
   ;	    byte_t F0 ;
   ;	    byte_t C0 ;
   ;	    byte_t congen0 ;
   ;	    byte_t visibilidadAnt ;
   ;	//  trama_t far * tramaProceso ;
   ;	
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	    setraw_DS() ;           /* establece el DS correspondiente al programa */
   ;	
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
   ;	
   ;	    /* punteros a pila (parametros/variables locales) */
   ;	    /*
   ;	      tramaProceso = MK_FP(*ptrSS_Proceso, *ptrSP_Proceso) ;
   ;	    */
   ;	    F0 = cursorF ;
   ;	
	mov	al,byte ptr DGROUP:_cursorF
	mov	byte ptr [bp-1],al
   ;	
   ;	    C0 = cursorC ;
   ;	
	mov	al,byte ptr DGROUP:_cursorC
	mov	byte ptr [bp-2],al
   ;	
   ;	    congen0 = congen ;
   ;	
	mov	al,byte ptr DGROUP:_congen
	mov	byte ptr [bp-3],al
   ;	
   ;	    visibilidadAnt = setCursorVisibilidad(0) ;
   ;	
	push	0
	call	near ptr _setCursorVisibilidad
	pop	cx
	mov	byte ptr [bp-4],al
   ;	
   ;	    if (consolaDeSuperficie != 0)
   ;	
	cmp	word ptr DGROUP:_consolaDeSuperficie,0
	je	short @29@8
   ;	
   ;	    {
   ;	        goToXYConsola(consolaDeSuperficie, 0, 75) ;
   ;	
	push	75
	push	0
	mov	al,byte ptr DGROUP:_consolaDeSuperficie
	push	ax
	call	near ptr _goToXYConsola
	add	sp,6
   ;	
   ;	//      printStrConsola(consolaDeSuperficie, "     ") ;
   ;	        printStrHastaConsola(consolaDeSuperficie, strCbBlancos, 5, TRUE) ;
   ;	
	push	1
	push	5
	push	ds
	push	offset DGROUP:_strCbBlancos
	mov	al,byte ptr DGROUP:_consolaDeSuperficie
	push	ax
	call	near ptr _printStrHastaConsola
	add	sp,10
   ;	
   ;	        goToXYConsola(consolaDeSuperficie, 1, 75) ;
   ;	
	push	75
	push	1
	mov	al,byte ptr DGROUP:_consolaDeSuperficie
	push	ax
	call	near ptr _goToXYConsola
	add	sp,6
   ;	
   ;	//      printStrConsola(consolaDeSuperficie, "CON") ;
   ;	        printStrConsola(consolaDeSuperficie, strCbCON) ;
   ;	
	push	ds
	push	offset DGROUP:_strCbCON
	mov	al,byte ptr DGROUP:_consolaDeSuperficie
	push	ax
	call	near ptr _printStrConsola
	add	sp,6
   ;	
   ;	        printIntConsola(consolaDeSuperficie, consolaDeSuperficie, 1) ;
   ;	
	push	1
	push	word ptr DGROUP:_consolaDeSuperficie
	mov	al,byte ptr DGROUP:_consolaDeSuperficie
	push	ax
	jmp	short @29@9
@29@8:
   ;	
   ;	    }
   ;	    else
   ;	    {
   ;	        goToXYConsola(0, 0, 64) ;
   ;	
	push	64
	push	0
	push	0
	call	near ptr _goToXYConsola
	add	sp,6
   ;	
   ;	//      printStrConsola(0, "                ") ;
   ;	//      printStrConsola(0, strCbBlancos) ;
   ;	        printStrHastaConsola(0, strCbBlancos, 16, TRUE) ;
   ;	
	push	1
	push	16
	push	ds
	push	offset DGROUP:_strCbBlancos
	push	0
	call	near ptr _printStrHastaConsola
	add	sp,10
   ;	
   ;	        goToXYConsola(0, 1, 64) ;
   ;	
	push	64
	push	1
	push	0
	call	near ptr _goToXYConsola
	add	sp,6
   ;	
   ;	        printLDecConsola(0, ((argCbTimer_t far *)arg)->contTics, 11) ;
   ;	
	push	11
	les	bx,dword ptr [bp+6]
	push	word ptr es:[bx+2]
	push	word ptr es:[bx]
	push	0
	call	near ptr _printLDecConsola
	add	sp,8
   ;	
   ;	        printIntConsola(0, ((argCbTimer_t far *)arg)->contTicsRodaja, 4) ;
   ;	
	push	4
	les	bx,dword ptr [bp+6]
	push	word ptr es:[bx+8]
	push	0
@29@9:
	call	near ptr _printIntConsola
	add	sp,6
   ;	
   ;	    }
   ;	    congen = congen0 ;
   ;	
	mov	al,byte ptr [bp-3]
	mov	byte ptr DGROUP:_congen,al
   ;	
   ;	    cursorF = F0 ;
   ;	
	mov	al,byte ptr [bp-1]
	mov	byte ptr DGROUP:_cursorF,al
   ;	
   ;	    cursorC = C0 ;
   ;	
	mov	al,byte ptr [bp-2]
	mov	byte ptr DGROUP:_cursorC,al
   ;	
   ;	    goToXYHw(cursorF, cursorC) ;
   ;	
	mov	al,byte ptr DGROUP:_cursorC
	push	ax
	mov	al,byte ptr DGROUP:_cursorF
	push	ax
	call	near ptr _goToXYHw
	add	sp,4
   ;	
   ;	    setCursorVisibilidad(visibilidadAnt) ;
   ;	
	mov	al,byte ptr [bp-4]
	mov	ah,0
	push	ax
	call	near ptr _setCursorVisibilidad
	pop	cx
   ;	
   ;	
   ;	    restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	    return(1) ;
   ;	
	mov	ax,1
   ;	
   ;	}
   ;	
	leave	
	ret	
_cbForTimer	endp
	assume	cs:_TEXT,ds:DGROUP
_finishConsola	proc	near
   ;	
   ;	int finishConsola ( void )
   ;	
   ;	
   ;	{
   ;	//  exit(0) ;              /* obligaria a meter la biblioteca ll_s_so1.lib */
   ;	    return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	ret	
_finishConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_finCodeDriver	proc	near
   ;	
   ;	void finCodeDriver ( void )   /* marca el fin del codigo propio del driver */
   ;	
   ;	
   ;	{
   ;	}
   ;	
	ret	
_finCodeDriver	endp
_TEXT	ends
_DATA	segment word public 'DATA'
_descCcbConsola	label	word
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_mostrarFormato	proc	near
   ;	
   ;	void mostrarFormato ( void )
   ;	
   ;	
   ;	{
   ;	    printf(" formato: CONSOLA [ [ -i | -q | -c | -l ] [ num ] | -N | -n | -u | -k | -h ] ") ;
   ;	
	push	ds
	push	offset DGROUP:s@+4
	call	near ptr _printf
	add	sp,4
   ;	
   ;	}
   ;	
	ret	
_mostrarFormato	endp
	assume	cs:_TEXT,ds:DGROUP
_formato	proc	near
   ;	
   ;	int formato ( void )
   ;	
   ;	
   ;	{
   ;	    printf("\n\n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+82
	call	near ptr _printf
	add	sp,4
   ;	
   ;	    mostrarFormato() ;
   ;	
	call	near ptr _mostrarFormato
   ;	
   ;	    printf("\n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+85
	call	near ptr _printf
	add	sp,4
   ;	
   ;	    return(-1) ;
   ;	
	mov	ax,-1
   ;	
   ;	}
   ;	
	ret	
_formato	endp
	assume	cs:_TEXT,ds:DGROUP
_help	proc	near
   ;	
   ;	int help ( void )
   ;	
   ;	
   ;	{
   ;	    printf("\n\n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+87
	call	near ptr _printf
	add	sp,4
   ;	
   ;	    mostrarFormato() ;
   ;	
	call	near ptr _mostrarFormato
   ;	
   ;	    printf(
   ;	
   ;	
   ;	        ""                                                               "\n"
   ;	        ""                                                               "\n"
   ;	        " instala/desinstala el driver de la consola"                    "\n"
   ;	        ""                                                               "\n"
   ;	        " opciones: (por defecto -i y num = 6)"                          "\n"
   ;	        ""                                                               "\n"
   ;	        "      -i  : instala el driver (usar &)"                         "\n"
   ;	        "      -q  : instala sin mensajes de salida (&)"                 "\n"
   ;	        "      -c  : cambia la consola actual a la num"                  "\n"
   ;	        "      -l  : establece en numero de lineas a num"                "\n"
   ;	        "      -n  : status = numero de consola actual"                  "\n"
   ;	        "      -nq : como -n pero sin mensaje por pantalla"              "\n"
   ;	        "      num : numero de consolas (o nueva c.)"                    "\n"
   ;	        "      -u  : desintala el driver"                                "\n"
   ;	        "      -k  : desintala el driver (matando)"                      "\n"
   ;	        "      -h  : muestra este help"                                  "\n"
   ;	    ) ;
   ;	
	push	ds
	push	offset DGROUP:s@+90
	call	near ptr _printf
	add	sp,4
   ;	
   ;	    return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	ret	
_help	endp
	assume	cs:_TEXT,ds:DGROUP
_hacerTablaNula	proc	near
   ;	
   ;	void hacerTablaNula ( char * tablaDeConversion ) 
   ;	
	enter	2,0
   ;	
   ;	{
   ;		int i ;
   ;		for ( i = 0 ; i < 256 ; i++ ) 
   ;	
	mov	word ptr [bp-2],0
	jmp	short @35@4
@35@2:
   ;	
   ;			tablaDeConversion[i] = (char)i ;
   ;	
	mov	bx,word ptr [bp+4]
	add	bx,word ptr [bp-2]
	mov	al,byte ptr [bp-2]
	mov	byte ptr [bx],al
	inc	word ptr [bp-2]
@35@4:
	cmp	word ptr [bp-2],256
	jl	short @35@2
   ;	
   ;	}
   ;	
	leave	
	ret	
_hacerTablaNula	endp
	assume	cs:_TEXT,ds:DGROUP
_establecerTablaDeConversion	proc	near
   ;	
   ;	void establecerTablaDeConversion ( void )
   ;	
	enter	4,0
   ;	
   ;	{
   ;	    int anioBIOS ;
   ;	
   ;	    anioBIOS = 10*(ptrFechaBios[6] - '0') + (ptrFechaBios[7] - '0') ;
   ;	
	mov	ax,-1
	mov	es,ax
	mov	al,byte ptr es:[12]
	mov	ah,0
	mov	dx,-1
	mov	es,dx
	mov	dl,byte ptr es:[11]
	mov	dh,0
	add	dx,-48
	imul	dx,dx,10
	add	ax,dx
	add	ax,-48
	mov	word ptr [bp-2],ax
   ;	
   ;	
   ;	    switch (anioBIOS)
   ;	
	mov	ax,word ptr [bp-2]
	mov	word ptr [bp-4],ax
	mov	cx,8
	mov	bx,offset @36@C18
@36@4:
	mov	ax,word ptr cs:[bx]
	cmp	ax,word ptr [bp-4]
	je	short @36@7
	add	bx,2
	loop	short @36@4
	jmp	short @36@9
@36@7:
	jmp	word ptr cs:[bx+16]
@36@8:
   ;	
   ;	    {
   ;	    case 99 : /* hayQemu   */   /* "06/23/99" */
   ;	    case 11 : /* hayBochs  */   /* "02/10/11" */            /* Bochs 2.6.7 */
   ;	    case 14 : /* hayBochs  */   /* "12/26/14" */            /* Bochs 2.6.8 */
   ;	    case 17 : /* hayBochs  */   /* "02/16/17" */            /* Bochs 2.6.9 */
   ;	    case 95 : /* hayNTVDM  */   /* "07/03/95" */
   ;	    case 12 : /* hayFake86 */   /* "05/02/12" */
   ;	//  case 99 : /* hayVDos   */   /* "01/01/99" */ 
   ;	        break ;                 
   ;	
	jmp	short @36@10
@36@9:
   ;	
   ;	    case  6 : /* hayMsdos  */                                    /* Takeda */
   ;	    case 92 : /* hayDBox   */   /* "01/01/92" */
   ;	    default  :
   ;	        hacerTablaNula(tablaDeConversion) ;                          /* US */
   ;	
	push	offset DGROUP:_tablaSP
	call	near ptr _hacerTablaNula
	pop	cx
@36@10:
   ;	
   ;	    }
   ;	}
   ;	
	leave	
	ret	
_establecerTablaDeConversion	endp
@36@C18	label	word
	db	6
	db	0
	db	11
	db	0
	db	12
	db	0
	db	14
	db	0
	db	17
	db	0
	db	92
	db	0
	db	95
	db	0
	db	99
	db	0
	dw	@36@9
	dw	@36@8
	dw	@36@8
	dw	@36@8
	dw	@36@8
	dw	@36@9
	dw	@36@8
	dw	@36@8
	assume	cs:_TEXT,ds:DGROUP
_integrarConsola	proc	near
   ;	
   ;	int integrarConsola ( byte_t numConsolas, bool_t conMensajes )
   ;	
	enter	106,0
   ;	
   ;	{
   ;	    dfs_t dfs ;
   ;	    descRecurso_t dR ;
   ;	    int dfTimer ;
   ;	    int res ;
   ;	    byte_t con ;
   ;	    pindx_t pindx ;
   ;	    char nomFich [ 12 ] ;
   ;	
   ;	//  obtenInfoINFO((info_t far *)&info) ;                     /* depuracion */
   ;	
   ;	    inicCrtHw() ;
   ;	
	call	near ptr _inicCrtHw
   ;	
   ;	
   ;	//  if (*((word_t far *)0xF000FFFB) == 0x3630)    /* anio fecha BIOS msdos */ /* Takeda */
   ;	    if (*((word_t far *)0xF000FFFB) == 0x3239)    /* anio fecha BIOS msdos */ /* Takeda */
   ;	
	mov	ax,-4096
	mov	es,ax
	cmp	word ptr es:[65531],12857
	jne	short @37@3
   ;	
   ;	//      inicBiosCrt(24, 80, TRUE) ;           /* se fuerza redimensionable */
   ;	    {
   ;	        inicBiosCrt(ptrBiosArea->VIDEO_lastrow + 1, 80, TRUE) ; /* se fuerza redimensionable */
   ;	
	push	1
	push	80
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	push	ax
	jmp	short @37@4
@37@3:
   ;	
   ;	    }
   ;	    else
   ;	        inicBiosCrt(50, 80, FALSE) ;       /* no se fuerza redimensionable */
   ;	
	push	0
	push	80
	push	50
@37@4:
	call	near ptr _inicBiosCrt
	add	sp,6
   ;	
   ;	
   ;	    establecerTablaDeConversion() ;                       /* teclado US/SP */
   ;	
	call	near ptr _establecerTablaDeConversion
   ;	
   ;	
   ;	    maxConsolas = numConsolas + 1 ;
   ;	
	mov	al,byte ptr [bp+4]
	inc	al
	mov	byte ptr DGROUP:_maxConsolas,al
   ;	
   ;	
   ;	#if (FALSE)
   ;	    descConsola = MK_FP(
   ;	                      ll_buscarBloque(((maxConsolas*sizeof(descConsola_t))+15)/16),  /* GM */
   ;	                      0x0000
   ;	                  ) ;
   ;	#else
   ;		/* se supone que el proceso inicial ha reservado antes el bloque */
   ;		descConsola = MK_FP(_CS-(((maxConsolas*sizeof(descConsola_t))+15)/16), 0x0000) ;
   ;	
	mov	al,byte ptr DGROUP:_maxConsolas
	mov	ah,0
	imul	ax,ax,8378
	add	ax,15
	shr	ax,4
	mov	dx,cs
	sub	dx,ax
	mov	word ptr DGROUP:_descConsola+2,dx
	mov	word ptr DGROUP:_descConsola,0
   ;	
   ;	#endif
   ;					  
   ;	/*
   ;	    printStrBIOS("\n descConsola = ") ;
   ;	    printPtrBIOS((pointer_t)descConsola) ;
   ;	    printStrBIOS("\n numFilas = ") ;
   ;	    printIntBIOS(maxFilasAct, 1) ;
   ;	    printStrBIOS(" numColumnas = ") ;
   ;	    printIntBIOS(numColumnas, 1) ;
   ;	*/
   ;	    for ( con = 0 ; con < maxConsolas ; con++ )
   ;	
	mov	byte ptr [bp-7],0
	jmp	@37@8
@37@6:
   ;	
   ;	    {
   ;	        descConsola[con].teclado.in = 0 ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	word ptr es:[bx+8340],0
   ;	
   ;	        descConsola[con].teclado.out = 0 ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	word ptr es:[bx+8342],0
   ;	
   ;	        descConsola[con].teclado.ncar = 0 ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	word ptr es:[bx+8344],0
   ;	
   ;	        inicPC2c(&descConsola[con].teclado.bloqueados,
   ;	                 &descConsola[con].teclado.e2Bloqueados, maxProcesos + 0, TRUE) ;
   ;	
	push	1
	push	16
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola
	add	dx,ax
	add	dx,8002
	push	word ptr DGROUP:_descConsola+2
	push	dx
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola
	add	dx,ax
	add	dx,8074
	push	word ptr DGROUP:_descConsola+2
	push	dx
	call	near ptr _inicializarPC2c
	add	sp,12
   ;	
   ;	        descConsola[con].teclado.inRaw = 0 ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	word ptr es:[bx+8372],0
   ;	
   ;	        descConsola[con].teclado.outRaw = 0 ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	word ptr es:[bx+8374],0
   ;	
   ;	        descConsola[con].teclado.nRaw = 0 ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	word ptr es:[bx+8376],0
   ;	
   ;	        inicPC2c(&descConsola[con].teclado.bloqueadosRaw,
   ;	                 &descConsola[con].teclado.e2Bloqueados, maxProcesos + 1, TRUE) ;
   ;	
	push	1
	push	17
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola
	add	dx,ax
	add	dx,8002
	push	word ptr DGROUP:_descConsola+2
	push	dx
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola
	add	dx,ax
	add	dx,8346
	push	word ptr DGROUP:_descConsola+2
	push	dx
	call	near ptr _inicializarPC2c
	add	sp,12
	inc	byte ptr [bp-7]
@37@8:
	mov	al,byte ptr [bp-7]
	cmp	al,byte ptr DGROUP:_maxConsolas
	jae	@@13
	jmp	@37@6
@@13:
   ;	
   ;	    }
   ;	
   ;	    consolaDeSuperficie = 0 ;
   ;	
	mov	word ptr DGROUP:_consolaDeSuperficie,0
   ;	
   ;	
   ;	    for ( con = 0 ; con < maxConsolas ; con++ )
   ;	
	mov	byte ptr [bp-7],0
	jmp	short @37@12
@37@10:
   ;	
   ;	    {
   ;	        borrarPantalla((pantalla_t far *)&descConsola[con].pantalla, maxFilas) ;
   ;	
	push	50
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	mov	dx,word ptr DGROUP:_descConsola
	add	dx,ax
	push	word ptr DGROUP:_descConsola+2
	push	dx
	call	near ptr _borrarPantalla
	add	sp,6
   ;	
   ;	        descConsola[con].F = 0 ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	byte ptr es:[bx+8000],0
   ;	
   ;	        descConsola[con].C = 0 ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	imul	ax,ax,8378
	les	bx,dword ptr DGROUP:_descConsola
	add	bx,ax
	mov	byte ptr es:[bx+8001],0
	inc	byte ptr [bp-7]
@37@12:
	mov	al,byte ptr [bp-7]
	cmp	al,byte ptr DGROUP:_maxConsolas
	jb	short @37@10
   ;	
   ;	    }
   ;	
   ;	    for ( pindx = 0 ; pindx < maxProcesos ; pindx++ )
   ;	
	mov	word ptr [bp-10],0
	jmp	short @37@16
@37@14:
   ;	
   ;	    {
   ;	        nbytesProceso[pindx] = 0 ;                /* nbytes esperando leer */
   ;	
	mov	bx,word ptr [bp-10]
	add	bx,bx
	mov	word ptr DGROUP:_nbytesProceso[bx],0
   ;	
   ;	        dirProceso[pindx] = NULL ;
   ;	
	mov	bx,word ptr [bp-10]
	shl	bx,2
	mov	word ptr DGROUP:_dirProceso[bx+2],0
	mov	word ptr DGROUP:_dirProceso[bx],0
	inc	word ptr [bp-10]
@37@16:
	cmp	word ptr [bp-10],16
	jl	short @37@14
   ;	
   ;	    }
   ;	
   ;	    dR.tipo = rDCaracteres ;
   ;	
	mov	word ptr [bp-82],1
   ;	
   ;	    strcpy(dR.nombre, "CONSOLA") ;
   ;	
	push	ds
	push	offset DGROUP:s@+601
	push	ss
	lea	ax,word ptr [bp-94]
	push	ax
	call	near ptr _strcpy
	add	sp,8
   ;	
   ;	    dR.ccb = (ccb_t)&descCcbConsola ;
   ;	
	mov	word ptr [bp-78],ds
	mov	word ptr [bp-80],offset DGROUP:_descCcbConsola
   ;	
   ;	    dR.ccb->arg = NULL ;
   ;	
	les	bx,dword ptr [bp-80]
	mov	word ptr es:[bx+10],0
	mov	word ptr es:[bx+8],0
   ;	
   ;	    dR.pindx = getpindx() ;
   ;	
	call	near ptr _getpindx
	mov	word ptr [bp-76],ax
   ;	
   ;	    dR.numVI = 1 ;
   ;	
	mov	byte ptr [bp-64],1
   ;	
   ;	    dR.nVInt[0] = nVIntConsola ;
   ;	
	mov	byte ptr [bp-63],9
   ;	
   ;	    dR.irq[0] = IRQ_TECLADO ;
   ;	
	mov	byte ptr [bp-61],1
   ;	
   ;	    dR.isr[0] = MK_FP(_CS, FP_OFF(isr_consola)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _isr_consola
	mov	word ptr [bp-57],ax
	mov	word ptr [bp-59],dx
   ;	
   ;	
   ;	    dR.open      = (open_t)     MK_FP(_CS, FP_OFF(openConsola)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _openConsola
	mov	word ptr [bp-49],ax
	mov	word ptr [bp-51],dx
   ;	
   ;	    dR.release   = (release_t)  MK_FP(_CS, FP_OFF(releaseConsola)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _releaseConsola
	mov	word ptr [bp-45],ax
	mov	word ptr [bp-47],dx
   ;	
   ;	    dR.read      = (read_t)     MK_FP(_CS, FP_OFF(readConsola)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _readConsola
	mov	word ptr [bp-41],ax
	mov	word ptr [bp-43],dx
   ;	
   ;	    dR.aio_read  = (aio_read_t) MK_FP(_CS, FP_OFF(aio_readConsola)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _aio_readConsola
	mov	word ptr [bp-37],ax
	mov	word ptr [bp-39],dx
   ;	
   ;	    dR.write     = (write_t)    MK_FP(_CS, FP_OFF(writeConsola)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _writeConsola
	mov	word ptr [bp-33],ax
	mov	word ptr [bp-35],dx
   ;	
   ;	    dR.aio_write = (aio_write_t)MK_FP(_CS, FP_OFF(aio_writeConsola)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _aio_writeConsola
	mov	word ptr [bp-29],ax
	mov	word ptr [bp-31],dx
   ;	
   ;	    dR.lseek     = (lseek_t)    MK_FP(_CS, FP_OFF(lseekConsola)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _lseekConsola
	mov	word ptr [bp-25],ax
	mov	word ptr [bp-27],dx
   ;	
   ;	    dR.fcntl     = (fcntl_t)    MK_FP(_CS, FP_OFF(fcntlConsola)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _fcntlConsola
	mov	word ptr [bp-21],ax
	mov	word ptr [bp-23],dx
   ;	
   ;	    dR.ioctl     = (ioctl_t)    MK_FP(_CS, FP_OFF(ioctlConsola)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _ioctlConsola
	mov	word ptr [bp-17],ax
	mov	word ptr [bp-19],dx
   ;	
   ;	
   ;	    dR.eliminar  = (eliminar_t) MK_FP(_CS, FP_OFF(eliminarConsola)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _eliminarConsola
	mov	word ptr [bp-13],ax
	mov	word ptr [bp-15],dx
   ;	
   ;	
   ;	    rec_consola = crearRecurso(&dR) ;
   ;	
	push	ss
	lea	ax,word ptr [bp-94]
	push	ax
	call	near ptr _crearRecurso
	add	sp,4
	mov	word ptr DGROUP:_rec_consola,ax
   ;	
   ;	
   ;	    if (rec_consola >= 0)
   ;	
	cmp	word ptr DGROUP:_rec_consola,0
	jge	@@14
	jmp	@37@41
@@14:
   ;	
   ;	    {
   ;	        strcpy(nomFich, "CON ") ;
   ;	
	push	ds
	push	offset DGROUP:s@+609
	push	ss
	lea	ax,word ptr [bp-106]
	push	ax
	call	near ptr _strcpy
	add	sp,8
   ;	
   ;	        for ( con = 0 ; con < maxConsolas ; con++ )
   ;	
	mov	byte ptr [bp-7],0
	jmp	@37@32
@37@19:
   ;	
   ;	        {
   ;	            nomFich[3] = '0' + con ;
   ;	
	mov	al,byte ptr [bp-7]
	add	al,48
	mov	byte ptr [bp-103],al
   ;	
   ;	            dfs = crearFichero(nomFich, rec_consola, con, fedCaracteres) ;
   ;	
	push	3
	mov	al,byte ptr [bp-7]
	mov	ah,0
	push	ax
	push	word ptr DGROUP:_rec_consola
	push	ss
	lea	ax,word ptr [bp-106]
	push	ax
	call	near ptr _crearFichero
	add	sp,10
	mov	word ptr [bp-2],ax
   ;	
   ;	            if (dfs < 0)
   ;	
	cmp	word ptr [bp-2],0
	jge	short @37@31
   ;	
   ;	            {
   ;	                switch(dfs)
   ;	
	mov	bx,word ptr [bp-2]
	sub	bx,-4
	cmp	bx,3
	ja	short @37@28
	add	bx,bx
	jmp	word ptr cs:@37@C148[bx]
@37@23:
   ;	
   ;	                {
   ;	                case -1 : printf(" tipo de fichero erroneo") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+614
	jmp	short @37@27
@37@24:
   ;	
   ;	                case -2 : printf(" numero de recurso erroneo") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+639
	jmp	short @37@27
@37@25:
   ;	
   ;	                case -3 : printf(" CON%i nombre de fichero ya existente", con) ; break ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	push	ax
	push	ds
	push	offset DGROUP:s@+666
	jmp	short @37@29
@37@26:
   ;	
   ;	                case -4 : printf(" no hay descriptores de fichero libres") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+704
@37@27:
	call	near ptr _printf
	add	sp,4
	jmp	short @37@30
@37@28:
   ;	
   ;	                default : printf(" no ha podido crearse el fichero CON%i", con) ;
   ;	
	mov	al,byte ptr [bp-7]
	mov	ah,0
	push	ax
	push	ds
	push	offset DGROUP:s@+743
@37@29:
	call	near ptr _printf
	add	sp,6
@37@30:
   ;	
   ;	                }
   ;	                destruirRecurso("CONSOLA", TRUE) ;              /* matando */
   ;	
	push	1
	push	ds
	push	offset DGROUP:s@+782
	call	near ptr _destruirRecurso
	add	sp,6
@37@31:
	inc	byte ptr [bp-7]
@37@32:
	mov	al,byte ptr [bp-7]
	cmp	al,byte ptr DGROUP:_maxConsolas
	jae	@@15
	jmp	@37@19
@@15:
   ;	
   ;	            }
   ;	        }
   ;	
   ;	        if ((dfTimer = open("TIMER", O_RDONLY)) >= 0)
   ;	
	push	1
	push	ds
	push	offset DGROUP:s@+790
	call	near ptr _open
	add	sp,6
	mov	word ptr [bp-4],ax
	or	ax,ax
	jl	short @37@37
   ;	
   ;	        {
   ;	            close(dfTimer) ;
   ;	
	push	word ptr [bp-4]
	call	near ptr _close
	pop	cx
   ;	
   ;	            res = encolarCcbRecurso(
   ;	
   ;	
   ;	                      (callBack_t)MK_FP(_CS, (word_t)cbForTimer),
   ;	                      (char far *)MK_FP(_DS, (word_t)&"TIMER")
   ;	                  ) ;
   ;	
	mov	ax,ds
	xor	dx,dx
	or	dx,offset DGROUP:s@+796
	push	ax
	push	dx
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _cbForTimer
	push	ax
	push	dx
	call	near ptr _encolarCcbRecurso
	add	sp,8
	mov	word ptr [bp-6],ax
   ;	
   ;	            if (res != 0)
   ;	
	cmp	word ptr [bp-6],0
	je	short @37@36
   ;	
   ;	                printf("\n\n no ha podido encolarse el callback en el TIMER \n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+802
	call	near ptr _printf
	add	sp,4
@37@36:
   ;	
   ;	        }
   ;	
	jmp	short @37@38
@37@37:
   ;	
   ;	        else
   ;	            printf("\n\n no ha podido encolarse el callback en el TIMER \n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+854
	call	near ptr _printf
	add	sp,4
@37@38:
   ;	
   ;	
   ;	        if (conMensajes)
   ;	
	cmp	word ptr [bp+6],0
	je	short @37@40
   ;	
   ;	            printf("\n\n recurso CONSOLA instalado (ficheros: CON0 ...) \n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+906
	call	near ptr _printf
	add	sp,4
@37@40:
   ;	
   ;	        return(0) ;
   ;	
	xor	ax,ax
	leave	
	ret	
@37@41:
   ;	
   ;	    }
   ;	
   ;	    printf("\n\n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+958
	call	near ptr _printf
	add	sp,4
   ;	
   ;	    switch(rec_consola)
   ;	
	mov	bx,word ptr DGROUP:_rec_consola
	sub	bx,-5
	cmp	bx,4
	ja	short @37@50
	add	bx,bx
	jmp	word ptr cs:@37@C153[bx]
@37@45:
   ;	
   ;	    {
   ;	    case -1 : printf(" tipo de recurso erroneo ") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+961
	jmp	short @37@51
@37@46:
   ;	
   ;	    case -2 : printf(" demasiados vectores de interrupcion ") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+987
	jmp	short @37@51
@37@47:
   ;	
   ;	    case -3 : printf(" CONSOLA nombre de recurso ya existente ") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+1025
	jmp	short @37@51
@37@48:
   ;	
   ;	    case -4 : printf(" no hay descriptores de recurso libres ") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+1066
	jmp	short @37@51
@37@49:
   ;	
   ;	    case -5 : printf(" numero de vector de interrupcion ya usado ") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+1106
	jmp	short @37@51
@37@50:
   ;	
   ;	    default : printf(" no ha podido crearse el recurso CONSOLA ") ;
   ;	
	push	ds
	push	offset DGROUP:s@+1150
@37@51:
	call	near ptr _printf
	add	sp,4
   ;	
   ;	    }
   ;	    printf("\n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+1192
	call	near ptr _printf
	add	sp,4
   ;	
   ;	    return(-1) ;
   ;	
	mov	ax,-1
   ;	
   ;	}
   ;	
	leave	
	ret	
_integrarConsola	endp
@37@C153	label	word
	dw	@37@49
	dw	@37@48
	dw	@37@47
	dw	@37@46
	dw	@37@45
@37@C148	label	word
	dw	@37@26
	dw	@37@25
	dw	@37@24
	dw	@37@23
	assume	cs:_TEXT,ds:DGROUP
_desintegrarConsola	proc	near
   ;	
   ;	int desintegrarConsola ( void )
   ;	
	enter	2,0
   ;	
   ;	{
   ;	    int res ;
   ;	    res = eliminarCcbRecurso(
   ;	
   ;	
   ;	              (callBack_t)MK_FP(_CS, (word_t)cbForTimer),
   ;	              (char far *)MK_FP(_DS, (word_t)&"TIMER")
   ;	          ) ;
   ;	
	mov	ax,ds
	xor	dx,dx
	or	dx,offset DGROUP:s@+1194
	push	ax
	push	dx
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _cbForTimer
	push	ax
	push	dx
	call	near ptr _eliminarCcbRecurso
	add	sp,8
	mov	word ptr [bp-2],ax
   ;	
   ;	    if (res != 0)
   ;	
	cmp	word ptr [bp-2],0
	je	short @38@3
   ;	
   ;	        printf(" no pudo eliminarse el callback del TIMER ") ;
   ;	
	push	ds
	push	offset DGROUP:s@+1200
	call	near ptr _printf
	add	sp,4
	jmp	short @38@5
@38@3:
   ;	
   ;	    else if (ll_devolverBloque(                                      /* GM */
   ;	
   ;	
   ;	                FP_SEG(descConsola),
   ;	                ((maxConsolas*sizeof(descConsola_t))+15)/16)
   ;	            )
   ;	
	mov	al,byte ptr DGROUP:_maxConsolas
	mov	ah,0
	imul	ax,ax,8378
	add	ax,15
	shr	ax,4
	push	ax
	push	word ptr DGROUP:_descConsola+2
	call	near ptr _ll_devolverBloque
	add	sp,4
	or	ax,ax
	je	short @38@5
   ;	
   ;	        return(-1) ;
   ;	
	mov	ax,-1
	leave	
	ret	
@38@5:
   ;	
   ;	/*  finBiosCrt() ; */
   ;	    return(res) ;
   ;	
	mov	ax,word ptr [bp-2]
   ;	
   ;	}
   ;	
	leave	
	ret	
_desintegrarConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_instalarConsola	proc	near
   ;	
   ;	int instalarConsola ( byte_t numConsolas, bool_t conMensajes )
   ;	
	enter	2,0
   ;	
   ;	{
   ;	    int res ;
   ;	    res = comprobarAmpersand() ;
   ;	
	call	near ptr _comprobarAmpersand
	mov	word ptr [bp-2],ax
   ;	
   ;	    if (res != 0) return(res) ;
   ;	
	cmp	word ptr [bp-2],0
	je	short @39@3
	jmp	short @39@6
@39@3:
   ;	
   ;	    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
   ;	
	push	ds
	push	offset DGROUP:_ptrIndProcesoActual
	call	near ptr _obtenInfoSO1
	add	sp,4
   ;	
   ;	    res = integrarConsola(numConsolas, conMensajes) ;
   ;	
	push	word ptr [bp+6]
	mov	al,byte ptr [bp+4]
	push	ax
	call	near ptr _integrarConsola
	add	sp,4
	mov	word ptr [bp-2],ax
   ;	
   ;	    if (res != 0) return(res) ;
   ;	
	cmp	word ptr [bp-2],0
	je	short @39@5
	jmp	short @39@6
@39@5:
   ;	
   ;	#if (!REDUCIR_DRIVER)
   ;	    esperarDesinstalacion(0) ;                       /* bloquea el proceso */
   ;	#else
   ;	    esperarDesinstalacion(                           /* bloquea el proceso */
   ;	
   ;	
   ;	//      FP_OFF(dirDescSO1) + sizeof(descSO1_t)
   ;	//          ...
   ;	//          + sizeof(descCcbConsola) + 0*sizeof(callBack_t),    /* tamDATA */
   ;	        FP_OFF(&descCcbConsola)
   ;	            + sizeof(descCcbConsola) + 0*sizeof(callBack_t),    /* tamDATA */
   ;	        FP_OFF(finCodeDriver),                            /* finCodeDriver */
   ;	        FP_OFF(finishConsola),                             /* finishDriver */
   ;			0x0000                                                  /* tamPila */ 
   ;	    ) ;
   ;	
	push	0
	push	offset _finishConsola
	push	offset _finCodeDriver
	mov	ax,offset DGROUP:_descCcbConsola
	add	ax,12
	push	ax
	call	near ptr _esperarDesinstalacion
	add	sp,8
   ;	
   ;	/*  se retorna a finishConsola */		
   ;	#endif
   ;	/*  solo se llega aqui en el caso esperarDesinstalacion(0) */
   ;	    res = desintegrarConsola() ;
   ;	
	call	near ptr _desintegrarConsola
	mov	word ptr [bp-2],ax
@39@6:
   ;	
   ;	    return(res) ;
   ;	
	mov	ax,word ptr [bp-2]
   ;	
   ;	}
   ;	
	leave	
	ret	
_instalarConsola	endp
	assume	cs:_TEXT,ds:DGROUP
_main	proc	near
   ;	
   ;	int main ( int argc, char * argv [ ] )
   ;	
	enter	6,0
   ;	
   ;	{
   ;		
   ;		pantalla_t far * p = MK_FP(0xB800,0x0000) ;        
   ;	
	mov	word ptr [bp-2],-18432
	mov	word ptr [bp-4],0
   ;	
   ;		byte_t F = 0 ;
   ;	
	mov	byte ptr [bp-5],0
   ;	
   ;		byte_t C = 0 ;
   ;	
	mov	byte ptr [bp-6],0
	jmp	short @40@5
@40@2:
   ;	
   ;		
   ;		while ((car = getchar()) != 'F') {
   ;			if (car == '\n') {
   ;	
	cmp	byte ptr DGROUP:_car,10
	jne	short @40@4
   ;	
   ;			    F++ ;
   ;	
	inc	byte ptr [bp-5]
   ;	
   ;				C = 0 ;
   ;	
	mov	byte ptr [bp-6],0
   ;	
   ;			}
   ;	
	jmp	short @40@5
@40@4:
   ;	
   ;			else {
   ;		        p->t[F][C].car = car ;
   ;	
	mov	al,byte ptr [bp-5]
	mov	ah,0
	imul	ax,ax,160
	les	bx,dword ptr [bp-4]
	add	bx,ax
	mov	al,byte ptr [bp-6]
	mov	ah,0
	add	ax,ax
	add	bx,ax
	mov	al,byte ptr DGROUP:_car
	mov	byte ptr es:[bx],al
   ;	
   ;		     	C++ ;
   ;	
	inc	byte ptr [bp-6]
@40@5:
	call	near ptr _getchar
	mov	byte ptr DGROUP:_car,al
	cmp	al,70
	jne	short @40@2
   ;	
   ;			}
   ;		}
   ;			
   ;	//	printf("\n\n Hola Takeda \n") ;
   ;		return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;		
   ;	#if (FALSE)	
   ;	    int res ;
   ;	    int dfCon ;
   ;	    if (argc > 3) return(formato()) ;
   ;	    else if (argc == 1) return(instalarConsola(numConsolasPorDefecto, TRUE)) ;
   ;	    else if (argc == 2)
   ;	    {
   ;	        if (argv[1][0] != '-') return(formato()) ;
   ;	        switch (toupper(argv[1][1]))
   ;	        {
   ;	        case 'H' :
   ;	            return(help()) ;
   ;	        case 'I' :
   ;	            return(instalarConsola(numConsolasPorDefecto, TRUE)) ;
   ;	        case 'Q' :
   ;	            return(instalarConsola(numConsolasPorDefecto, FALSE)) ;
   ;	        case 'N' :
   ;	            if ((dfCon = open("CON0", O_RDONLY)) < 0)
   ;	            {
   ;	                printf(" no puede abrirse CON0") ;
   ;	                return(-1) ;
   ;	            }
   ;	            else
   ;	            {
   ;	                res = ioctl(dfCon, 0x0002, 0x0000) ;
   ;	                if (res < 0) printf(" error al consultar la consola actual ") ;
   ;	                else
   ;	                {
   ;	                    if (toupper(argv[1][2]) != 'Q') /* (-nq no muestra mensaje) */
   ;	                        printf(" la consola actual es CON%i", res) ;
   ;	                }
   ;	                return(res) ;
   ;	            }
   ;	        case 'U' :
   ;	        case 'K' :
   ;	            res = destruirRecurso("CONSOLA", tolower(argv[1][1]) == 'k') ; 
   ;	            switch (res)
   ;	            {
   ;	            case  0 : printf(" recurso CONSOLA desinstalado") ; break ;
   ;	            case -1 : printf(" recurso CONSOLA no existe") ; break ;
   ;	            case -2 : printf(" recurso CONSOLA en uso") ; break ;
   ;	            case -3 : printf(" fichero CONSOLA no puede destruirse") ; break ;
   ;	            default : printf(" CONSOLA no ha podido desinstalarse") ;
   ;	            }
   ;	            return(res) ;
   ;	        default :
   ;	            ;
   ;	        }
   ;	    }
   ;	    if ((argc == 2) ||
   ;	        ((argc == 3) &&
   ;	         (!strcmpu(argv[1], "-i") || !strcmpu(argv[1], "-q"))
   ;	        )
   ;	       )
   ;	    {
   ;	        strcpy(comando[0], argv[argc-1]) ;
   ;	        inicScanner() ;
   ;	        obtenSimb() ;
   ;	        if (simb == s_numero)
   ;	        {
   ;	            if (num == 0)
   ;	            {
   ;	                printf("\n\n el numero de consolas debe ser > 0 \n") ;
   ;	                return(-1) ;
   ;	            }
   ;	            else return(instalarConsola(num, (toupper(argv[1][1]) == 'Q'))) ;
   ;	        }
   ;	    }
   ;	    else if (argc == 3)
   ;	        if (!strcmpu(argv[1], "-c"))
   ;	        {
   ;	            strcpy(comando[0], argv[argc-1]) ;
   ;	            inicScanner() ;
   ;	            obtenSimb() ;
   ;	            if (simb == s_numero)
   ;	            {
   ;	                if ((dfCon = open("CON0", O_RDONLY)) < 0)
   ;	                {
   ;	                    printf(" no puede abrirse CON0") ;
   ;	                    return(-1) ;
   ;	                }
   ;	                if (ioctl(dfCon, 0x0001, num) != 0)
   ;	                {
   ;	                    printf(" numero de consola erroneo (%i)", num) ;
   ;	                    return(-1) ;
   ;	                }
   ;	                return(0) ;
   ;	            }
   ;	        }
   ;	        else if (!strcmpu(argv[1], "-l"))
   ;	        {
   ;	            strcpy(comando[0], argv[argc-1]) ;
   ;	            inicScanner() ;
   ;	            obtenSimb() ;
   ;	            if (simb == s_numero)
   ;	            {
   ;	                if ((num != 25) && (num != 28) && (num != 50)) {
   ;	                    printf(" el numero de lineas debe ser 25, 28 o 50") ;
   ;	                    return(-1) ;
   ;	                }
   ;	                if ((dfCon = open("CON0", O_RDONLY)) < 0)
   ;	                {
   ;	                    printf(" no pudo abrirse CON0") ;
   ;	                    return(-1) ;
   ;	                }
   ;	                if (ioctl(dfCon, 0x0003, num) == -1)
   ;	                {
   ;	                    printf(" numero de consola erroneo (%i)", num) ;
   ;	                    return(-1) ;
   ;	                }
   ;	                return(0) ;
   ;	            }
   ;	        }
   ;	    return(formato()) ;
   ;	#endif
   ;		
   ;	}
   ;	
	leave	
	ret	
_main	endp
_TEXT	ends
_BSS	segment word public 'BSS'
_info	label	word
	db	12 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	7
	db	0
	db	7
	db	0
	db	' formato: CONSOLA [ [ -i | -q | -c | -l ] [ num ] | -N | -n |'
	db	' -u | -k | -h ] '
	db	0
	db	10
	db	10
	db	0
	db	10
	db	0
	db	10
	db	10
	db	0
	db	10
	db	10
	db	' instala/desinstala el driver de la consola'
	db	10
	db	10
	db	' opciones: (por defecto -i y num = 6)'
	db	10
	db	10
	db	'      -i  : instala el driver (usar &)'
	db	10
	db	'      -q  : instala sin mensajes de salida (&)'
	db	10
	db	'      -c  : cambia la consola actual a la num'
	db	10
	db	'      -l  : establece en numero de lineas a num'
	db	10
	db	'      -n  : status = numero de consola actual'
	db	10
	db	'      -nq : como -n pero sin mensaje por pantalla'
	db	10
	db	'      num : numero de consolas (o nueva c.)'
	db	10
	db	'      -u  : desintala el driver'
	db	10
	db	'      -k  : desintala el driver (matando)'
	db	10
	db	'      -h  : muestra este help'
	db	10
	db	0
	db	'CONSOLA'
	db	0
	db	'CON '
	db	0
	db	' tipo de fichero erroneo'
	db	0
	db	' numero de recurso erroneo'
	db	0
	db	' CON%i nombre de fichero ya existente'
	db	0
	db	' no hay descriptores de fichero libres'
	db	0
	db	' no ha podido crearse el fichero CON%i'
	db	0
	db	'CONSOLA'
	db	0
	db	'TIMER'
	db	0
	db	'TIMER'
	db	0
	db	10
	db	10
	db	' no ha podido encolarse el callback en el TIMER '
	db	10
	db	0
	db	10
	db	10
	db	' no ha podido encolarse el callback en el TIMER '
	db	10
	db	0
	db	10
	db	10
	db	' recurso CONSOLA instalado (ficheros: CON0 ...) '
	db	10
	db	0
	db	10
	db	10
	db	0
	db	' tipo de recurso erroneo '
	db	0
	db	' demasiados vectores de interrupcion '
	db	0
	db	' CONSOLA nombre de recurso ya existente '
	db	0
	db	' no hay descriptores de recurso libres '
	db	0
	db	' numero de vector de interrupcion ya usado '
	db	0
	db	' no ha podido crearse el recurso CONSOLA '
	db	0
	db	10
	db	0
	db	'TIMER'
	db	0
	db	' no pudo eliminarse el callback del TIMER '
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	extrn	_eliminarPC2c:near
	extrn	_encolarPC2c:near
	extrn	_desencolarPC2c:near
	extrn	_inicializarPC2c:near
	extrn	_car:byte
	extrn	_comprobarAmpersand:near
	extrn	_open:near
	extrn	_close:near
	extrn	_getpindx:near
	extrn	_obtenInfoSO1:near
	extrn	_crearRecurso:near
	extrn	_crearFichero:near
	extrn	_esperarDesinstalacion:near
	extrn	_destruirRecurso:near
	extrn	_encolarCcbRecurso:near
	extrn	_eliminarCcbRecurso:near
	extrn	_ll_devolverBloque:near
	extrn	_printf:near
	extrn	_getchar:near
	extrn	_strcpy:near
	extrn	_leerTeclaListaBDA:near
	extrn	_teclaListaBDA:near
	extrn	_cambiarTeclaListaBDA:near
	extrn	_borrarPantalla:near
	extrn	_borrarCPantalla:near
	extrn	_copiarCPantalla:near
	extrn	_scrollCPantalla:near
	extrn	_ptrPant:dword
	extrn	_redimensionable:word
	extrn	_inicBiosCrt:near
	extrn	_setCursorVisibilidad:near
	extrn	_redimensionar:near
	extrn	_inicCrtHw:near
	extrn	_goToXYHw:near
	extrn	_cursorF:byte
	extrn	_cursorC:byte
	extrn	_printCarVideo:near
	extrn	_printGenLn:near
	extrn	_printGenStr:near
	extrn	_printGenStrHasta:near
	extrn	_printGenDec:near
	extrn	_printGenLDec:near
	extrn	_printGenInt:near
	extrn	_printGenLInt:near
	extrn	_printGenHex:near
	extrn	_printGenLHex:near
	extrn	_printGenPtr:near
	public	_tablaSP
	public	_info
	public	_rec_consola
	public	_descConsola
	public	_maxConsolas
	public	_consolaDeSuperficie
	public	_nbytesProceso
	public	_dirProceso
	public	_meter
	public	_sacar
	public	_printCarConsola
	public	_goToXYConsola
	public	_establecerConsolaDeSuperficie
	public	_salvarConsolaDeSuperficie
	public	_openConsola
	public	_releaseConsola
	public	_readConsola
	public	_aio_readConsola
	public	_writeConsola
	public	_aio_writeConsola
	public	_lseekConsola
	extrn	N_LMOD@:far
	extrn	N_LDIV@:far
	public	_fcntlConsola
	public	_ioctlConsola
	public	_eliminarConsola
	public	_keyStroke
	public	_scanCode
	public	_scanCodeAnt
	public	_AltPulsada
	public	_CtrlPulsada
	public	_isr_consola
	public	_congen
	public	_pCon
	public	_printLnConsola
	public	_printStrConsola
	public	_printStrHastaConsola
	public	_printDecConsola
	public	_printIntConsola
	public	_printLIntConsola
	public	_printLDecConsola
	public	_printHexConsola
	public	_printLHexConsola
	public	_printPtrConsola
	public	_strCbBlancos
	public	_strCbCON
	public	_cbForTimer
	public	_finishConsola
	public	_finCodeDriver
	public	_descCcbConsola
	public	_mostrarFormato
	public	_formato
	public	_help
	public	_hacerTablaNula
	public	_establecerTablaDeConversion
	public	_integrarConsola
	public	_desintegrarConsola
	public	_instalarConsola
	public	_main
	extrn	__setargv__:far
	end
