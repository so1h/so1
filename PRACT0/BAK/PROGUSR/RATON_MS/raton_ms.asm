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
	?debug	S "raton_ms.c"
	?debug	C E9A8156A4C0A7261746F6E5F6D732E63
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
	?debug	C E9BCAE624C20583A5C534F5C5052414354305C736F317075622E68+
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
	?debug	C E969A04F4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C65736372696269722E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9A088164B1B583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6332632E68
	?debug	C E90E885A4C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C63747970652E68
	?debug	C E9C7705B4C1F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C737472696E67732E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9149D71481F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7363616E6E65722E68
	?debug	C E9450B664C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C62696F73646174612E68
	?debug	C E97400554C1E583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C62696F735F302E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9EA02594C1E583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6D656D6F72792E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9E4106A4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C70616E74616C6C612E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E9F6036A4C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F70616E742E68
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
	?debug	C E92D644F4A1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6D73646F732E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E90871694C1F583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C6465665F7261742E68
	?debug	C E95992694C20583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C62696F735F7261742E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E97400554C1E583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C62696F735F302E68
	?debug	C E901937C481D583A5C534F5C5052414354305C736F312E685C696E+
	?debug	C 7465727275702E68
	?debug	C E93BBB674C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C7469706F732E68
	?debug	C E92FBE554C1D583A5C534F5C5052414354305C736F317075622E68+
	?debug	C 5C636F6E73742E68
_rec_raton	label	word
	db	0
	db	0
_bloqueadosRaton	label	word
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
_e2BloqueadosRaton	label	word
	db	0
	db	0
	db	0
	db	0
	db	64 dup (0)
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
_inB	label	word
	db	0
	db	0
_B	label	byte
	db	0
	db	0
	db	0
_er	label	word
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
	db	1 dup (0)
_XAux	label	word
	db	0
	db	0
_YAux	label	word
	db	0
	db	0
_FAntCursor	label	word
	db	0
	db	0
_CAntCursor	label	word
	db	0
	db	0
_atrAux	label	byte
	db	0
	db	1 dup (0)
_cursorRatonActivo	label	word
	db	1
	db	0
_moviendo	label	word
	db	0
	db	0
_redimensionando	label	word
	db	0
	db	0
_primerClick	label	word
	db	1
	db	0
_contTics2Click	label	word
	db	0
	db	0
_huboInt	label	word
	db	1
	db	0
_primeraInt	label	word
	db	1
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_inicEstadoRaton	proc	near
	?debug	C E8010A7261746F6E5F6D732E63A8156A4C
	?debug	C E82720583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C616A7573747573722E68CF81654C
	?debug	C E8021F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66696E636F64652E68149D7148
	?debug	C E827
	?debug	C E80321583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7374617274646174612E68149D7148
	?debug	C E827
	?debug	C E8041F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66696E646174612E68149D7148
	?debug	C E827
	?debug	C E8051E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66696E6273732E68149D7148
	?debug	C E827
	?debug	C E801
	?debug	C E80E20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C636F6D756E6472762E68D5BE664C
	?debug	C E82820583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C616A7573747573722E68CF81654C
	?debug	C E80E
	?debug	C E8061D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80E
	?debug	C E8071B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6332632E68A088164B
	?debug	C E8291D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E807
	?debug	C E80E
	?debug	C E8081E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7074726332632E68149D7148
	?debug	C E82A1B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6332632E68A088164B
	?debug	C E808
	?debug	C E80E
	?debug	C E80D20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70726F632E68ACBC634C
	?debug	C E82B1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80D
	?debug	C E82C1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C636F6E73742E682FBE554C
	?debug	C E80D
	?debug	C E8091D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66636E746C2E685CB65A4A
	?debug	C E82D1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E809
	?debug	C E80D
	?debug	C E80A1F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F74726D2E68149D7148
	?debug	C E82E1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80A
	?debug	C E80D
	?debug	C E80B1F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7363616E6E65722E68149D7148
	?debug	C E82F1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80B
	?debug	C E80D
	?debug	C E8301B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6332632E68A088164B
	?debug	C E80D
	?debug	C E80C1B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6363622E6826A6664C
	?debug	C E8311D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80C
	?debug	C E80D
	?debug	C E80E
	?debug	C E801
	?debug	C E81620583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6C6C5F735F736F312E68BCAE624C
	?debug	C E8321D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E816
	?debug	C E83320583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70726F632E68ACBC634C
	?debug	C E816
	?debug	C E80F1F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F6D656D2E68FB06FC46
	?debug	C E8341D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E80F
	?debug	C E816
	?debug	C E8101E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F73662E683DA1554C
	?debug	C E8351D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E810
	?debug	C E816
	?debug	C E8131F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F7261742E680871694C
	?debug	C E8361D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E813
	?debug	C E81220583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70616E742E68F6036A4C
	?debug	C E8371D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E812
	?debug	C E81120583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C62696F73646174612E68450B664C
	?debug	C E8381D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E811
	?debug	C E812
	?debug	C E813
	?debug	C E816
	?debug	C E8391D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C66636E746C2E685CB65A4A
	?debug	C E816
	?debug	C E81420583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6C6C5F735F6578632E68CD01534C
	?debug	C E83A1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E814
	?debug	C E83B20583A5C534F5C5052414354305C736F3170+
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
	?debug	C E81820583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C65736372696269722E6869A04F4C
	?debug	C E83C1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E818
	?debug	C E801
	?debug	C E83D1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E801
	?debug	C E83E1B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6332632E68A088164B
	?debug	C E801
	?debug	C E8191D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C63747970652E680E885A4C
	?debug	C E801
	?debug	C E81A1F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C737472696E67732E68C7705B4C
	?debug	C E83F1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81A
	?debug	C E801
	?debug	C E8401F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7363616E6E65722E68149D7148
	?debug	C E801
	?debug	C E84120583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C62696F73646174612E68450B664C
	?debug	C E801
	?debug	C E81B1E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C62696F735F302E687400554C
	?debug	C E8421D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81B
	?debug	C E801
	?debug	C E81C1E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6D656D6F72792E68EA02594C
	?debug	C E8431D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81C
	?debug	C E801
	?debug	C E81D20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C70616E74616C6C612E68E4106A4C
	?debug	C E8441D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81D
	?debug	C E84520583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F70616E742E68F6036A4C
	?debug	C E81D
	?debug	C E801
	?debug	C E81E20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6D656D766964656F2E68C8066A4C
	?debug	C E8461D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81E
	?debug	C E84720583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C70616E74616C6C612E68E4106A4C
	?debug	C E81E
	?debug	C E801
	?debug	C E81F20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F7465636C2E686DB0694C
	?debug	C E8481D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E81F
	?debug	C E8491E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7074726332632E68149D7148
	?debug	C E81F
	?debug	C E84A20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F7363616E2E68149D7148
	?debug	C E84B1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E84A
	?debug	C E81F
	?debug	C E84C1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C636F6E73742E682FBE554C
	?debug	C E81F
	?debug	C E84D20583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C62696F73646174612E68450B664C
	?debug	C E81F
	?debug	C E801
	?debug	C E8211B583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7069632E68B900484A
	?debug	C E8201F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C70756572746F732E68FB06FC46
	?debug	C E84E1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E820
	?debug	C E821
	?debug	C E84F1D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E821
	?debug	C E801
	?debug	C E82221583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F74696D65722E6802A5664C
	?debug	C E8501D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E822
	?debug	C E801
	?debug	C E82320583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7072696E7467656E2E68D29B5A4C
	?debug	C E8511D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E823
	?debug	C E801
	?debug	C E8241D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6D73646F732E682D644F4A
	?debug	C E8521D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E824
	?debug	C E801
	?debug	C E8531F583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C6465665F7261742E680871694C
	?debug	C E801
	?debug	C E82520583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C62696F735F7261742E685992694C
	?debug	C E8541D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E825
	?debug	C E8551E583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C62696F735F302E687400554C
	?debug	C E825
	?debug	C E801
	?debug	C E8261D583A5C534F5C5052414354305C736F312E+
	?debug	C 685C696E7465727275702E6801937C48
	?debug	C E8561D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C7469706F732E683BBB674C
	?debug	C E826
	?debug	C E8571D583A5C534F5C5052414354305C736F3170+
	?debug	C 75622E685C636F6E73742E682FBE554C
	?debug	C E826
	?debug	C E801
   ;	
   ;	void inicEstadoRaton ( void )
   ;	
   ;	
   ;	{
   ;	    er.S0 = 0x00 ;                                    /* byte estado 8042  */
   ;	
	mov	byte ptr DGROUP:_er,0
   ;	
   ;	    er.B0 = 0x00 ;                                    /* Byte 0 = 00SS1MRL */
   ;	
	mov	byte ptr DGROUP:_er+1,0
   ;	
   ;	    er.B1 = 0x00 ;                                    /* Byte 1 = X        */
   ;	
	mov	byte ptr DGROUP:_er+2,0
   ;	
   ;	    er.B2 = 0x00 ;                                    /* Byte 2 = Y        */
   ;	
	mov	byte ptr DGROUP:_er+3,0
   ;	
   ;	    er.W0 = 0x0000 ;
   ;	
	mov	word ptr DGROUP:_er+4,0
   ;	
   ;	    er.X = maxX/2 ;
   ;	
	mov	word ptr DGROUP:_er+10,320
   ;	
   ;	//  er.Y = maxY/2 ; 
   ;	    er.Y = (maxYAct + 1)/2 ;	
   ;	
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	shl	ax,3
	inc	ax
	cwd	
	sub	ax,dx
	sar	ax,1
	mov	word ptr DGROUP:_er+12,ax
   ;	
   ;	    er.F = er.Y >> 3 ;                                          /* F = Y/8 */
   ;	
	mov	ax,word ptr DGROUP:_er+12
	sar	ax,3
	mov	word ptr DGROUP:_er+14,ax
   ;	
   ;	    er.C = er.X >> 3 ;                                          /* C = X/8 */
   ;	
	mov	ax,word ptr DGROUP:_er+10
	sar	ax,3
	mov	word ptr DGROUP:_er+16,ax
   ;	
   ;	    er.botonIz = FALSE ;
   ;	
	mov	word ptr DGROUP:_er+18,0
   ;	
   ;	    er.botonDe = FALSE ;
   ;	
	mov	word ptr DGROUP:_er+20,0
   ;	
   ;	    er.botonMe = FALSE ;
   ;	
	mov	word ptr DGROUP:_er+22,0
   ;	
   ;	
   ;	    primeraInt = TRUE ;
   ;	
	mov	word ptr DGROUP:_primeraInt,1
   ;	
   ;	    cursorRatonActivo = TRUE ;
   ;	
	mov	word ptr DGROUP:_cursorRatonActivo,1
   ;	
   ;	    primerClick = TRUE ;
   ;	
	mov	word ptr DGROUP:_primerClick,1
   ;	
   ;	    contTics2Click = 0 ;
   ;	
	mov	word ptr DGROUP:_contTics2Click,0
   ;	
   ;	}
   ;	
	ret	
_inicEstadoRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_leerEstadoRaton	proc	near
   ;	
   ;	bool_t leerEstadoRaton ( estadoRaton_t far * estado )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    memcpy(estado, &er, sizeof(estadoRaton_t)) ;
   ;	
	push	34
	push	ds
	push	offset DGROUP:_er
	push	word ptr [bp+6]
	push	word ptr [bp+4]
	call	near ptr _memcpy
	add	sp,10
   ;	
   ;	    if (huboInt)
   ;	
	cmp	word ptr DGROUP:_huboInt,0
	je	short @2@5
   ;	
   ;	    {
   ;	        huboInt = FALSE ;
   ;	
	mov	word ptr DGROUP:_huboInt,0
   ;	
   ;	        return(TRUE) ;
   ;	
	mov	ax,1
	pop	bp
	ret	
   ;	
   ;	    }
   ;	
	jmp	short @2@6
@2@5:
   ;	
   ;	    else
   ;	        return(FALSE) ;
   ;	
	xor	ax,ax
@2@6:
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_leerEstadoRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_openRaton	proc	far
   ;	
   ;	int far openRaton ( int dfs, modoAp_t modo )
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
_openRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_releaseRaton	proc	far
   ;	
   ;	int far releaseRaton ( int dfs )
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
_releaseRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_readRaton	proc	far
   ;	
   ;	int far readRaton ( int dfs, pointer_t dir, word_t nbytes )
   ;	
	enter	4,0
   ;	
   ;	{
   ;	    word_t nbARetornar ;
   ;	    pindx_t indProcesoActual ;
   ;	
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	 	setraw_DS() ;           /* establece el DS correspondiente al programa */
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
	mov	word ptr [bp-4],ax
   ;	
   ;	    if (nbytes <= sizeof(estadoRaton_t))
   ;	
	cmp	word ptr [bp+12],34
	ja	short @5@8
   ;	
   ;	        nbARetornar = nbytes ;
   ;	
	mov	ax,word ptr [bp+12]
	mov	word ptr [bp-2],ax
	jmp	short @5@9
@5@8:
   ;	
   ;	    else
   ;	        nbARetornar = sizeof(estadoRaton_t) ;
   ;	
	mov	word ptr [bp-2],34
@5@9:
   ;	
   ;	    memcpy(dir, (pointer_t)&er, nbARetornar) ;
   ;	
	push	word ptr [bp-2]
	push	ds
	push	offset DGROUP:_er
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	call	near ptr _memcpy
	add	sp,10
   ;	
   ;	
   ;	    nbytesProceso[indProcesoActual] = nbARetornar ; /* nbytes esperando leer */
   ;	
	mov	bx,word ptr [bp-4]
	add	bx,bx
	mov	ax,word ptr [bp-2]
	mov	word ptr DGROUP:_nbytesProceso[bx],ax
   ;	
   ;	    dirProceso[indProcesoActual] = dir ;
   ;	
	mov	bx,word ptr [bp-4]
	shl	bx,2
	mov	dx,word ptr [bp+10]
	mov	ax,word ptr [bp+8]
	mov	word ptr DGROUP:_dirProceso[bx+2],dx
	mov	word ptr DGROUP:_dirProceso[bx],ax
   ;	
   ;	    encolarPC2c(indProcesoActual, &bloqueadosRaton) ;
   ;	
	push	ds
	push	offset DGROUP:_bloqueadosRaton
	push	word ptr [bp-4]
	call	near ptr _encolarPC2c
	add	sp,6
   ;	
   ;	    bloquearProcesoActual(rec_raton) ;
   ;	
	push	word ptr DGROUP:_rec_raton
	call	dword ptr DGROUP:_bloquearProcesoActual
	pop	cx
   ;	
   ;	
   ;		restore_DS0() ;                               /* restaura el DS de SO1 */
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
_readRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_aio_readRaton	proc	far
   ;	
   ;	int far aio_readRaton ( int dfs, pointer_t dir, word_t nbytes )
   ;	
	enter	2,0
   ;	
   ;	{
   ;	    word_t nbARetornar ;
   ;	
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	 	setraw_DS() ;           /* establece el DS correspondiente al programa */
   ;	
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
   ;	
   ;	
   ;	    if (nbytes <= sizeof(estadoRaton_t))
   ;	
	cmp	word ptr [bp+12],34
	ja	short @6@8
   ;	
   ;	        nbARetornar = nbytes ;
   ;	
	mov	ax,word ptr [bp+12]
	mov	word ptr [bp-2],ax
	jmp	short @6@9
@6@8:
   ;	
   ;	    else
   ;	        nbARetornar = sizeof(estadoRaton_t) ;
   ;	
	mov	word ptr [bp-2],34
@6@9:
   ;	
   ;	    memcpy(dir, &er, nbARetornar) ;
   ;	
	push	word ptr [bp-2]
	push	ds
	push	offset DGROUP:_er
	push	word ptr [bp+10]
	push	word ptr [bp+8]
	call	near ptr _memcpy
	add	sp,10
   ;	
   ;	
   ;		restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	    return(nbARetornar) ;
   ;	
	mov	ax,word ptr [bp-2]
   ;	
   ;	}
   ;	
	leave	
	ret	
_aio_readRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_writeRaton	proc	far
   ;	
   ;	int far writeRaton ( int dfs, pointer_t dir, word_t nbytes )
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
_writeRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_aio_writeRaton	proc	far
   ;	
   ;	int far aio_writeRaton ( int dfs, pointer_t dir, word_t nbytes )
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
_aio_writeRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_lseekRaton	proc	far
   ;	
   ;	long int far lseekRaton ( int dfs, long int pos, word_t whence )
   ;	
   ;	
   ;	{
   ;	    return((long int)-1) ;
   ;	
	mov	dx,-1
	mov	ax,-1
   ;	
   ;	}
   ;	
	ret	
_lseekRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_fcntlRaton	proc	far
   ;	
   ;	int far fcntlRaton ( int dfs, word_t cmd, word_t arg )
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
_fcntlRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_ioctlRaton	proc	far
   ;	
   ;	int far ioctlRaton ( int dfs, word_t request, word_t arg )
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
_ioctlRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_eliminarRaton	proc	far
   ;	
   ;	int far eliminarRaton ( pindx_t pindx )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	 	setraw_DS() ;           /* establece el DS correspondiente al programa */
   ;	
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
   ;	
   ;		
   ;	    if (estaPC2c(pindx, &bloqueadosRaton))
   ;	
	mov	ax,word ptr [bp+6]
	shl	ax,2
	les	bx,dword ptr DGROUP:_bloqueadosRaton+6
	add	bx,ax
	cmp	word ptr es:[bx],-1
	je	short @12@8
   ;	
   ;	        eliminarPC2c(pindx, &bloqueadosRaton) ;
   ;	
	push	ds
	push	offset DGROUP:_bloqueadosRaton
	push	word ptr [bp+6]
	call	near ptr _eliminarPC2c
	add	sp,6
@12@8:
   ;	
   ;	
   ;		restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	    return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_eliminarRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_isr_raton	proc	far
   ;	
   ;	void far isr_raton ( void )
   ;	
   ;	
   ;	{
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	 	set_DS() ;              /* establece el DS correspondiente al programa */
   ;	
 	push	 ax 
 	push	 dx 
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
 	pop	 dx 
 	pop	 ax 
   ;	
   ;	
   ;	//                                    /*  plot('m', 0, contadorTimer0()) ; */
   ;	    tratarRaton() ;
   ;	
	call	near ptr _tratarRaton
   ;	
   ;	//                                    /*  plot('e', 0, contadorTimer0()) ; */
   ;	    eoi_pic2() ;
   ;	
 	mov	 al,020H 
 	out	 000A0H,al 
 	mov	 al,020H 
 	out	 00020H,al 
   ;	
   ;	
   ;		restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	}
   ;	
	ret	
_isr_raton	endp
	assume	cs:_TEXT,ds:DGROUP
_isr_raton_dosbox	proc	far
   ;	
   ;	void far isr_raton_dosbox ( void )
   ;	
   ;	
   ;	{
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	 	set_DS() ;              /* establece el DS correspondiente al programa */
   ;	
 	push	 ax 
 	push	 dx 
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
 	pop	 dx 
 	pop	 ax 
   ;	
   ;	
   ;	    ptrVIOrg[nVIntRaton]() ;    /* llamamos a la antigua rti (pushf + int) */
   ;	
	les	bx,dword ptr DGROUP:_ptrVIOrg
	pushf	
	call	dword ptr es:[bx+464]
   ;	
   ;	//                                    /*  plot('m', 0, contadorTimer0()) ; */
   ;	    procesarRaton((word_t)er.B0, er.X, er.Y) ;
   ;	
	push	word ptr DGROUP:_er+12
	push	word ptr DGROUP:_er+10
	mov	al,byte ptr DGROUP:_er+1
	mov	ah,0
	push	ax
	call	near ptr _procesarRaton
	add	sp,6
   ;	
   ;	//                                    /*  plot('e', 0, contadorTimer0()) ; */
   ;	//  eoi_pic2() ; 
   ;	
   ;		restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	}
   ;	
	ret	
_isr_raton_dosbox	endp
_TEXT	ends
_DATA	segment word public 'DATA'
_rH_CS	label	word
	db	0
	db	0
_rH_IP	label	word
	db	0
	db	0
_rH_AX	label	word
	db	0
	db	0
_rH_BX	label	word
	db	0
	db	0
_rH_CX	label	word
	db	0
	db	0
_rH_DX	label	word
	db	0
	db	0
_rH_SI	label	word
	db	0
	db	0
_rH_DI	label	word
	db	0
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_isr_raton_BIOS	proc	far
   ;	
   ;	void far isr_raton_BIOS ( void )
   ;	
   ;	
   ;	{
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	 	set_DS() ;              /* establece el DS correspondiente al programa */
   ;	
 	push	 ax 
 	push	 dx 
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
 	pop	 dx 
 	pop	 ax 
   ;	
   ;	
   ;	//                                    /*  plot('m', 0, contadorTimer0()) ; */
   ;	    procesarRaton(rH_BX, rH_CX, rH_DX)  ;
   ;	
	push	word ptr DGROUP:_rH_DX
	push	word ptr DGROUP:_rH_CX
	push	word ptr DGROUP:_rH_BX
	call	near ptr _procesarRaton
	add	sp,6
   ;	
   ;	//                                    /*  plot('e', 0, contadorTimer0()) ; */
   ;	//  eoi_pic2() ;
   ;	
   ;		restore_DS0() ;                               /* restaura el DS de SO1 */
   ;	
 	pop	 ds 
   ;	
   ;	}
   ;	
	ret	
_isr_raton_BIOS	endp
_TEXT	ends
_DATA	segment word public 'DATA'
_rti_nVIntRaton	label	dword
	db	0
	db	0
	db	0
	db	0
_segment_rti_nVIntRaton	label	word
	db	0
	db	0
_offset_rti_nVIntRaton	label	word
	db	0
	db	0
_ptrWordAux	label	dword
	db	0
	db	0
	db	0
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_rti_raton_BIOS	proc	far
   ;	
   ;	void far rti_raton_BIOS ( void )
   ;	
   ;	
   ;	{
   ;	    asm {		
   ;			sub sp,10 ;                             /* Flags, CS, IP (Proceso) */
   ;	
	sub	 sp,10 
   ;	
   ;	//                                               /* segment_rti_nVIntRaton */
   ;	//                                                /* offset_rti_nVIntRaton */
   ;	    }
   ;	    save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	
 	push	 ds 
   ;	
   ;	 	set_DS() ;              /* establece el DS correspondiente al programa */
   ;	
 	push	 ax 
 	push	 dx 
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
 	pop	 dx 
 	pop	 ax 
   ;	
   ;	
   ;	    asm {				  
   ;	        push ax ;
   ;	
	push	 ax 
   ;	
   ;	        push bp ;
   ;	
	push	 bp 
   ;	
   ;	
   ;	        mov bp,sp ;
   ;	
	mov	 bp,sp 
   ;	
   ;	        mov ax,rH_CS ;                                     /* CS (Proceso) */  
   ;	
	mov	 ax,DGROUP:_rH_CS 
   ;	
   ;	        mov ss:[bp+12],ax ;   
   ;	
	mov	 ss:[bp+12],ax 
   ;	
   ;	        mov ax,rH_IP ;
   ;	
	mov	 ax,DGROUP:_rH_IP 
   ;	
   ;	        mov ss:[bp+10],ax ;                                /* IP (Proceso) */
   ;	
	mov	 ss:[bp+10],ax 
   ;	
   ;	        mov ax,segment_rti_nVIntRaton ;
   ;	
	mov	 ax,DGROUP:_segment_rti_nVIntRaton 
   ;	
   ;	        mov ss:[bp+8],ax ;                       /* segment_rti_nVIntRaton */      
   ;	
	mov	 ss:[bp+8],ax 
   ;	
   ;	        mov ax,offset_rti_nVIntRaton ;
   ;	
	mov	 ax,DGROUP:_offset_rti_nVIntRaton 
   ;	
   ;	        mov ss:[bp+6],ax ;                        /* offset_rti_nVIntRaton */
   ;	
	mov	 ss:[bp+6],ax 
   ;	
   ;		}
   ;	/*
   ;	        printStrBIOS("\n isr_raton_BIOS") ;
   ;	        ptrWordAux = (word_t far  *)MK_FP(_SS,_SP) ;
   ;	        printStrBIOS("\n ") ;
   ;	        printHexBIOS(ptrWordAux[0], 4) ;
   ;	        printStrBIOS("\n ") ;
   ;	        printHexBIOS(ptrWordAux[1], 4) ;
   ;	        printStrBIOS("\n ") ;
   ;	        printHexBIOS(ptrWordAux[2], 4) ;
   ;	        printStrBIOS("\n ") ;
   ;	        printHexBIOS(ptrWordAux[3], 4) ;
   ;	        printStrBIOS("\n ") ;
   ;	        printHexBIOS(ptrWordAux[4], 4) ;
   ;	        printStrBIOS("\n ") ;
   ;	        printHexBIOS(ptrWordAux[5], 4) ;
   ;	        printStrBIOS("\n ") ;
   ;	        printHexBIOS(ptrWordAux[6], 4) ;
   ;	        printStrBIOS("\n ") ;
   ;	        printHexBIOS(ptrWordAux[7], 4) ;
   ;	        printStrBIOS("\n ") ;
   ;	        printHexBIOS(ptrWordAux[8], 4) ;
   ;	        printStrBIOS("\n ") ;
   ;	*/
   ;	    asm {
   ;	        pop bp ;
   ;	
	pop	 bp 
   ;	
   ;	        pop ax ;
   ;	
	pop	 ax 
   ;	
   ;	        /* retorno lejano a rti_nVIntRaton */
   ;		}
   ;		restore_DS0() ;                               /* restaura el DS de SO1 */	
   ;	
 	pop	 ds 
   ;	
   ;	}
   ;	
	ret	
_rti_raton_BIOS	endp
	assume	cs:_TEXT,ds:DGROUP
_ratonHandler	proc	far
   ;	
   ;	void far ratonHandler ( void )           /* ver techelp int 33h AH = 000Ch */
   ;	
   ;	
   ;	{
   ;	    /* Se llama a esta funcion tras la interrupcion y mediante un CALL FAR */
   ;	    /* Por ese motivo la pila contiene lo siguiente:                       */
   ;		/*                                                                     */
   ;		/*     Flags, CS, IP del proceso y direccion de retorno (lejana)       */
   ;		/*                                                                     */
   ;		/* Es innecesario establecer DS (ver techelp int 33h 000Ch)            */
   ;		
   ;	
   ;	//  save_DS0() ;                            /* guarda el DS anterior (SO1) */
   ;	// 	set_DS() ;              /* establece el DS correspondiente al programa */
   ;		
   ;	    asm {
   ;			mov rH_AX,ax ;        /* mascara de eventos ocurridos, ver techelp */
   ;	
	mov	 DGROUP:_rH_AX,ax 
   ;	
   ;	        mov rH_BX,bx ;                                /* botones: 00000MRL */
   ;	
	mov	 DGROUP:_rH_BX,bx 
   ;	
   ;	        mov rH_CX,cx ;                                                /* X */
   ;	
	mov	 DGROUP:_rH_CX,cx 
   ;	
   ;	        mov rH_DX,dx ;                                                /* Y */
   ;	
	mov	 DGROUP:_rH_DX,dx 
   ;	
   ;	//      mov rH_SI,si ;                                             /* incX */
   ;	//      mov rH_DI,di ;                                             /* incY */
   ;	
   ;	    /* Con lo siguente modificamos la trama de la pila de manera que la    */
   ;	    /* interrupcion en vez de retornar al CS:IP del proceso, retorne a la  */
   ;	    /* a la rutina rti_raton_BIOS. El CS:IP del proceso quedan guardados   */
   ;	    /* en las variables rH_CS:rH_IP con el fin de poder retornar en un     */
   ;	    /* momento posterior al proceso.                                       */     
   ;	
   ;	        push ax ;
   ;	
	push	 ax 
   ;	
   ;	        push bp ;
   ;	
	push	 bp 
   ;	
   ;	        mov bp,sp ;
   ;	
	mov	 bp,sp 
   ;	
   ;	        mov ax,ss:[bp+12] ;                              /* CS del proceso */
   ;	
	mov	 ax,ss:[bp+12] 
   ;	
   ;	        mov rH_CS,ax ;                                   
   ;	
	mov	 DGROUP:_rH_CS,ax 
   ;	
   ;	        mov ax,ss:[bp+10] ;                              /* IP del proceso */
   ;	
	mov	 ax,ss:[bp+10] 
   ;	
   ;	        mov rH_IP,ax ;       
   ;	
	mov	 DGROUP:_rH_IP,ax 
   ;	
   ;	        mov ss:[bp+12],cs ;                                          /* CS */
   ;	
	mov	 ss:[bp+12],cs 
   ;	
   ;	        mov ax,OFFSET rti_raton_BIOS ;                   /* rti_raton_BIOS */
   ;	
	mov	 ax,OFFSET _rti_raton_BIOS 
   ;	
   ;	        mov ss:[bp+10],ax ;
   ;	
	mov	 ss:[bp+10],ax 
   ;	
   ;	        pop bp ;
   ;	
	pop	 bp 
   ;	
   ;	        pop ax ;
   ;	
	pop	 ax 
   ;	
   ;	    }
   ;	//	restore_DS0() ;                               /* restaura el DS de SO1 */	
   ;	}                            /* de el a isr_raton_bios y de ahi al proceso */
   ;	
	ret	
_ratonHandler	endp
_TEXT	ends
_DATA	segment word public 'DATA'
$aoobdaia	label	byte
	db	0
$ibpbdaia	label	byte
	db	0
$afpbdaia	label	word
	db	0
	db	0
$iipbdaia	label	word
	db	0
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_tratarRaton	proc	near
   ;	
   ;	void tratarRaton ( void )
   ;	
   ;	
   ;	{
   ;	    static byte_t byteEstado = 0 ;                                 /* DATA */
   ;	    static byte_t byteDato = 0 ;                                   /* DATA */
   ;	    static int incX = 0, incY = 0 ;                                /* DATA */
   ;	
   ;	    asm in al,64h ;                                      /* TECLADO_ESTADO */
   ;	
 	in	 al,64h 
   ;	
   ;	    asm mov byteEstado,al
   ;	
 	mov	 $aoobdaia,al
   ;	
   ;	    if ((byteEstado & 0x21) == 0x21)      /* hay byte procedente del raton */
   ;	
	mov	al,byte ptr DGROUP:$aoobdaia
	mov	ah,0
	and	ax,33
	cmp	ax,33
	je	@@0
	jmp	@18@28
@@0:
   ;	
   ;	    {
   ;	        er.S0 = byteEstado ;
   ;	
	mov	al,byte ptr DGROUP:$aoobdaia
	mov	byte ptr DGROUP:_er,al
   ;	
   ;	        asm in al,60h ;                                    /* TECLADO_DATO */
   ;	
 	in	 al,60h 
   ;	
   ;	        asm mov byteDato,al ;
   ;	
 	mov	 $ibpbdaia,al 
   ;	
   ;	
   ;	        if ((inB != 0) || (((byteDato^0x08) & 0xC8) == 0))
   ;	
	cmp	word ptr DGROUP:_inB,0
	jne	short @18@8
	mov	al,byte ptr DGROUP:$ibpbdaia
	xor	al,8
	test	al,200
	je	@@1
	jmp	@18@28
@@1:
@18@8:
   ;	
   ;	        {
   ;	            B[inB] = byteDato ;
   ;	
	mov	bx,word ptr DGROUP:_inB
	mov	al,byte ptr DGROUP:$ibpbdaia
	mov	byte ptr DGROUP:_B[bx],al
   ;	
   ;	            inB = (inB + 1) % 3 ;
   ;	
	mov	ax,word ptr DGROUP:_inB
	inc	ax
	mov	bx,3
	cwd	
	idiv	bx
	mov	word ptr DGROUP:_inB,dx
   ;	
   ;	
   ;	            if (inB == 0)
   ;	
	cmp	word ptr DGROUP:_inB,0
	je	@@2
	jmp	@18@28
@@2:
   ;	
   ;	            {
   ;	
   ;	                er.B0 = B[0] ;
   ;	
	mov	al,byte ptr DGROUP:_B
	mov	byte ptr DGROUP:_er+1,al
   ;	
   ;	                er.B1 = B[1] ;
   ;	
	mov	al,byte ptr DGROUP:_B+1
	mov	byte ptr DGROUP:_er+2,al
   ;	
   ;	                if (er.B0 & 0x10)
   ;	
	test	byte ptr DGROUP:_er+1,16
	je	short @18@14
   ;	
   ;	                {
   ;	                    asm mov al, byte ptr er.B1 ;
   ;	
 	mov	 al, byte ptr DGROUP:_er+2 
   ;	
   ;	                    asm mov ah,0ffh ;
   ;	
 	mov	 ah,0ffh 
   ;	
   ;	                    asm mov incX,ax ;
   ;	
 	mov	 $afpbdaia,ax 
   ;	
   ;	                    er.incX = incX ;
   ;	
	mov	ax,word ptr DGROUP:$afpbdaia
	mov	word ptr DGROUP:_er+6,ax
   ;	
   ;	                }
   ;	
	jmp	short @18@15
@18@14:
   ;	
   ;	                else er.incX = er.B1 ;
   ;	
	mov	al,byte ptr DGROUP:_er+2
	mov	ah,0
	mov	word ptr DGROUP:_er+6,ax
@18@15:
   ;	
   ;	                XAux = er.X + er.incX ;
   ;	
	mov	ax,word ptr DGROUP:_er+10
	add	ax,word ptr DGROUP:_er+6
	mov	word ptr DGROUP:_XAux,ax
   ;	
   ;	                if ((0 <= XAux) && (XAux < maxX))            /* 8*80 = 640 */
   ;	
	cmp	word ptr DGROUP:_XAux,0
	jl	short @18@18
	cmp	word ptr DGROUP:_XAux,640
	jge	short @18@18
   ;	
   ;	                    er.X = XAux ;
   ;	
	mov	ax,word ptr DGROUP:_XAux
	mov	word ptr DGROUP:_er+10,ax
@18@18:
   ;	
   ;	
   ;	                er.B2 = B[2] ;
   ;	
	mov	al,byte ptr DGROUP:_B+2
	mov	byte ptr DGROUP:_er+3,al
   ;	
   ;	                if (er.B0 & 0x20)
   ;	
	test	byte ptr DGROUP:_er+1,32
	je	short @18@23
   ;	
   ;	                {
   ;	                    asm mov al, byte ptr er.B2 ;
   ;	
 	mov	 al, byte ptr DGROUP:_er+3 
   ;	
   ;	                    asm mov ah,0ffh ;
   ;	
 	mov	 ah,0ffh 
   ;	
   ;	                    asm mov incY,ax ;
   ;	
 	mov	 $iipbdaia,ax 
   ;	
   ;	                    er.incY = incY ;
   ;	
	mov	ax,word ptr DGROUP:$iipbdaia
	mov	word ptr DGROUP:_er+8,ax
   ;	
   ;	                }
   ;	
	jmp	short @18@24
@18@23:
   ;	
   ;	                else er.incY = er.B2 ;
   ;	
	mov	al,byte ptr DGROUP:_er+3
	mov	ah,0
	mov	word ptr DGROUP:_er+8,ax
@18@24:
   ;	
   ;	                er.incY = -er.incY ;                                /* ojo */
   ;	
	mov	ax,word ptr DGROUP:_er+8
	neg	ax
	mov	word ptr DGROUP:_er+8,ax
   ;	
   ;	                YAux = er.Y + er.incY ;
   ;	
	mov	ax,word ptr DGROUP:_er+12
	add	ax,word ptr DGROUP:_er+8
	mov	word ptr DGROUP:_YAux,ax
   ;	
   ;	                if ((0 <= YAux) && (YAux < maxYAct))         /* 8*25 = 200 */
   ;	
	cmp	word ptr DGROUP:_YAux,0
	jl	short @18@27
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	shl	ax,3
	cmp	ax,word ptr DGROUP:_YAux
	jle	short @18@27
   ;	
   ;	                    er.Y = YAux ;
   ;	
	mov	ax,word ptr DGROUP:_YAux
	mov	word ptr DGROUP:_er+12,ax
@18@27:
   ;	
   ;	
   ;	                procesarRaton((word_t)er.B0, er.X, er.Y) ;
   ;	
	push	word ptr DGROUP:_er+12
	push	word ptr DGROUP:_er+10
	mov	al,byte ptr DGROUP:_er+1
	mov	ah,0
	push	ax
	call	near ptr _procesarRaton
	add	sp,6
@18@28:
   ;	
   ;	            }
   ;	        }
   ;	    }
   ;	}
   ;	
	ret	
_tratarRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_procesarRaton	proc	near
   ;	
   ;	void procesarRaton ( word_t parW0, int parX, int parY )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	
   ;	//  static int posWF ;
   ;	//  static int posWC ;
   ;	//  static int FReal ;
   ;	//  static int CReal ;
   ;	
   ;	    er.W0 = parW0 ;
   ;	
	mov	ax,word ptr [bp+4]
	mov	word ptr DGROUP:_er+4,ax
   ;	
   ;	    er.X = parX ;
   ;	
	mov	ax,word ptr [bp+6]
	mov	word ptr DGROUP:_er+10,ax
   ;	
   ;	    er.Y = parY ;
   ;	
	mov	ax,word ptr [bp+8]
	mov	word ptr DGROUP:_er+12,ax
   ;	
   ;	
   ;	    er.F = er.Y >> 3 ;                                          /* F = Y/8 */
   ;	
	mov	ax,word ptr DGROUP:_er+12
	sar	ax,3
	mov	word ptr DGROUP:_er+14,ax
   ;	
   ;	    er.C = er.X >> 3 ;                                          /* C = X/8 */
   ;	
	mov	ax,word ptr DGROUP:_er+10
	sar	ax,3
	mov	word ptr DGROUP:_er+16,ax
   ;	
   ;	
   ;	    er.B0 = (byte_t)(er.W0 & 0x00FF) ;
   ;	
	mov	al,byte ptr DGROUP:_er+4
	and	al,255
	mov	byte ptr DGROUP:_er+1,al
   ;	
   ;	
   ;	    huboInt = TRUE ;             /* obtenidos nuevos valores para el raton */
   ;	
	mov	word ptr DGROUP:_huboInt,1
   ;	
   ;	
   ;	    er.botonIzAnt = er.botonIz ;
   ;	
	mov	ax,word ptr DGROUP:_er+18
	mov	word ptr DGROUP:_er+24,ax
   ;	
   ;	    er.botonDeAnt = er.botonDe ;
   ;	
	mov	ax,word ptr DGROUP:_er+20
	mov	word ptr DGROUP:_er+26,ax
   ;	
   ;	    er.botonMeAnt = er.botonMe ;
   ;	
	mov	ax,word ptr DGROUP:_er+22
	mov	word ptr DGROUP:_er+28,ax
   ;	
   ;	
   ;	    er.botonIz = (er.W0 & 0x0001) ;
   ;	
	mov	ax,word ptr DGROUP:_er+4
	and	ax,1
	mov	word ptr DGROUP:_er+18,ax
   ;	
   ;	    er.botonDe = ((er.W0 & 0x0002) >> 1) ;
   ;	
	mov	ax,word ptr DGROUP:_er+4
	and	ax,2
	shr	ax,1
	mov	word ptr DGROUP:_er+20,ax
   ;	
   ;	    er.botonMe = ((er.W0 & 0x0004) >> 2) ;
   ;	
	mov	ax,word ptr DGROUP:_er+4
	and	ax,4
	shr	ax,2
	mov	word ptr DGROUP:_er+22,ax
   ;	
   ;	
   ;	    procesarColaBloqueadosRaton() ;
   ;	
	call	near ptr _procesarColaBloqueadosRaton
   ;	
   ;	
   ;	    if (cursorRatonActivo)
   ;	
	cmp	word ptr DGROUP:_cursorRatonActivo,0
	je	short @19@9
   ;	
   ;	    {
   ;	        if (er.botonIz)                      /* boton izquierdo presionado */
   ;	
	cmp	word ptr DGROUP:_er+18,0
	je	short @19@4
   ;	
   ;	        {
   ;	#if (FALSE)
   ;	            if (moviendo || redimensionando)
   ;	            {
   ;	                if (moviendo)                                  /* moviendo */
   ;	                {
   ;	                    posWF = win->esqSupIzF ;
   ;	                    posWC = win->esqSupIzC ;
   ;	                    moverWin(win, er.F-FAntCursor, er.C-CAntCursor,
   ;	                             er.F-FAntCursor, er.C-CAntCursor) ;
   ;	                    FReal = FAntCursor + (win->esqSupIzF - posWF) ; /* F Real */
   ;	                    CReal = CAntCursor + (win->esqSupIzC - posWC) ; /* C Real */
   ;	                }
   ;	                else                                    /* redimensionando */
   ;	                {
   ;	                    posWF = win->esqInfDeF ;
   ;	                    posWC = win->esqInfDeC ;
   ;	                    moverWin(win, 0, 0, er.F-FAntCursor, er.C-CAntCursor) ;
   ;	                    FReal = FAntCursor + (win->esqInfDeF - posWF) ; /* F Real */
   ;	                    CReal = CAntCursor + (win->esqInfDeC - posWC) ; /* C Real */
   ;	                }
   ;	                if ((FReal != er.F) || (CReal != er.C))
   ;	                {
   ;	                    er.Y = (FReal << 3) + 3 ;                     /* ajuste Y */
   ;	                    er.X = (CReal << 3) + 3 ;                     /* ajuste X */
   ;	                    if (tipoRaton == msdos)
   ;	                        situarCursorRatonBIOS(er.X, er.Y) ;
   ;	                }
   ;	                er.F = FReal ;
   ;	                er.C = CReal ;
   ;	                actualizarCursor() ;
   ;	            }
   ;	            else if ((nwin = winVisible[er.F][er.C]) != 0)
   ;	            {
   ;	                if (!er.botonIzAnt)    /* primera pulsacion del boton izdo */
   ;	                {
   ;	                    win = winNum[nwin] ;
   ;	                    pindx = win->pindx ;
   ;	
   ;	                    if (er.F == win->esqSupIzF-1)        /* marco superior */
   ;	                    {
   ;	                        if (er.C == win->esqInfDeC-1) botonCerrarVentana() ;       /* [X] */
   ;	                        else if (er.C == win->esqInfDeC-4) maxMinWin(win, FALSE) ; /* [f] */
   ;	                        else if ((er.F == win->esqSupIzF-1) &&
   ;	                                 (win->esqSupIzC <= er.C) &&
   ;	                                 (er.C <= win->esqSupIzC+2+2*(win->pindx!=0)))
   ;	                            maxMinWin(win, TRUE) ;                                 /* max */
   ;	                        else
   ;	                            tratarClickMarco() ;
   ;	                    }
   ;	                    else if ((er.F == win->esqInfDeF+1) &&
   ;	                             (er.C == win->esqInfDeC+1))  /* redimensionar */
   ;	                    {
   ;	                        focalizarWin(win) ;
   ;	                        redimensionando = TRUE ;
   ;	                        actualizarCursor() ;
   ;	                    }
   ;	                    else if ((er.F == win->esqInfDeF+1) ||
   ;	                             (er.C == win->esqSupIzC-1) ||
   ;	                             (er.C == win->esqInfDeC+1))     /* otro marco */
   ;	                        tratarClickMarco() ;
   ;	                    else focalizarWin(win) ;             /* punto interior */
   ;	                }
   ;	                else actualizarCursor() ;
   ;	            }
   ;	#endif
   ;	        }
   ;	
	jmp	short @19@9
@19@4:
   ;	
   ;	        else                              /* boton izquierdo no presionado */
   ;	        {
   ;	            if (moviendo)
   ;	
	cmp	word ptr DGROUP:_moviendo,0
	je	short @19@6
   ;	
   ;	                moviendo = FALSE ;
   ;	
	mov	word ptr DGROUP:_moviendo,0
	jmp	short @19@8
@19@6:
   ;	
   ;	            else if (redimensionando)
   ;	
	cmp	word ptr DGROUP:_redimensionando,0
	je	short @19@8
   ;	
   ;	                redimensionando = FALSE ;
   ;	
	mov	word ptr DGROUP:_redimensionando,0
@19@8:
   ;	
   ;	            actualizarCursor() ;
   ;	
	call	near ptr _actualizarCursor
@19@9:
   ;	
   ;	        }
   ;	    }
   ;	}
   ;	
	pop	bp
	ret	
_procesarRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_procesarColaBloqueadosRaton	proc	near
   ;	
   ;	void procesarColaBloqueadosRaton ( void )
   ;	
	enter	4,0
	push	si
   ;	
   ;	{
   ;	
   ;	    pindx_t indProcesoActual ;
   ;	    pindx_t pindx ;
   ;	
   ;	    if (bloqueadosRaton.numElem > 0)
   ;	
	cmp	word ptr DGROUP:_bloqueadosRaton,0
	jg	@@3
	jmp	@20@6
@@3:
   ;	
   ;	    {
   ;	        pindx = bloqueadosRaton.primero ;
   ;	
	mov	ax,word ptr DGROUP:_bloqueadosRaton+2
	mov	word ptr [bp-4],ax
   ;	
   ;	        indProcesoActual = *ptrIndProcesoActual ;
   ;	
	les	bx,dword ptr DGROUP:_ptrIndProcesoActual
	mov	ax,word ptr es:[bx]
	mov	word ptr [bp-2],ax
   ;	
   ;	        if (indProcesoActual > -1)                          /* nivel 1 o 2 */
   ;	
	cmp	word ptr [bp-2],-1
	jle	short @20@4
   ;	
   ;	        {
   ;	            ptrDescProceso[indProcesoActual].trama =
   ;	
   ;	
   ;	                (trama_t far *)MK_FP(*ptrSS_Proceso, *ptrSP_Proceso) ;
   ;	
	les	bx,dword ptr DGROUP:_ptrSS_Proceso
	mov	ax,word ptr es:[bx]
	xor	dx,dx
	les	bx,dword ptr DGROUP:_ptrSP_Proceso
	or	dx,word ptr es:[bx]
	mov	bx,word ptr [bp-2]
	imul	bx,bx,220
	les	si,dword ptr DGROUP:_ptrDescProceso
	add	si,bx
	mov	word ptr es:[si+8],ax
	mov	word ptr es:[si+6],dx
   ;	
   ;	            ptrDescProceso[indProcesoActual].estado = preparado ;
   ;	
	mov	ax,word ptr [bp-2]
	imul	ax,ax,220
	les	bx,dword ptr DGROUP:_ptrDescProceso
	add	bx,ax
	mov	word ptr es:[bx+2],1
   ;	
   ;	            encolarPC2c(indProcesoActual, (ptrC2c_t)&(ptrC2cPFR[PPreparados])) ;
   ;	
	mov	ax,word ptr DGROUP:_ptrC2cPFR
	add	ax,20
	push	word ptr DGROUP:_ptrC2cPFR+2
	push	ax
	push	word ptr [bp-2]
	call	near ptr _encolarPC2c
	add	sp,6
   ;	
   ;	            *ptrIndProcesoActual = -1 ;
   ;	
	les	bx,dword ptr DGROUP:_ptrIndProcesoActual
	mov	word ptr es:[bx],-1
   ;	
   ;	            *ptrActivarAlEpilogo = TRUE ;
   ;	
	les	bx,dword ptr DGROUP:_ptrActivarAlEpilogo
	mov	word ptr es:[bx],1
@20@4:
   ;	
   ;	        }
   ;	
   ;	        do
   ;	        {
   ;	            memcpy(dirProceso[pindx], &er, nbytesProceso[pindx]) ;
   ;	
	mov	bx,word ptr [bp-4]
	add	bx,bx
	push	word ptr DGROUP:_nbytesProceso[bx]
	push	ds
	push	offset DGROUP:_er
	mov	bx,word ptr [bp-4]
	shl	bx,2
	push	word ptr DGROUP:_dirProceso[bx+2]
	push	word ptr DGROUP:_dirProceso[bx]
	call	near ptr _memcpy
	add	sp,10
   ;	
   ;	
   ;	#if (FALSE)
   ;	
   ;	            switch (descProceso[pindx].trama->DX)
   ;	            {
   ;	            case 0 :
   ;	                if (descProceso[pindx].win != descProceso[indProcesoDeSuperficie].win) break ;
   ;	            case 1 :
   ;	                if (winVisible[estado->F][estado->C] != descProceso[pindx].win->num) break ;
   ;	            case 2 :
   ;	
   ;	                switch (descProceso[pindx].esperandoPor)
   ;	                {
   ;	                case rec_tecladoRaton :
   ;	                    eliminarPC2c(pindx, &descProceso[pindx].teclado->bloqueados) ;
   ;	                    descProceso[pindx].trama->AL = (char)0 ;
   ;	                    break ;
   ;	                case rec_tecladoRawRaton :
   ;	                    eliminarPC2c(pindx, &descProceso[pindx].teclado->bloqueadosRaw) ;
   ;	                    descProceso[pindx].trama->AX = 0x0000 ;
   ;	                    descProceso[pindx].trama->BX = 0x0000 ;           /* modificamos BX */
   ;	                    break ;
   ;	                }
   ;	#endif
   ;	                ptrDescProceso[pindx].estado = preparado ;
   ;	
	mov	ax,word ptr [bp-4]
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
	push	word ptr [bp-4]
	call	near ptr _encolarPC2c
	add	sp,6
   ;	
   ;	#if (FALSE)
   ;	
   ;	            }
   ;	
   ;	#endif
   ;	
   ;	            desencolarPC2c(&bloqueadosRaton) ;
   ;	
	push	ds
	push	offset DGROUP:_bloqueadosRaton
	call	near ptr _desencolarPC2c
	add	sp,4
   ;	
   ;	            pindx = bloqueadosRaton.primero ;
   ;	
	mov	ax,word ptr DGROUP:_bloqueadosRaton+2
	mov	word ptr [bp-4],ax
   ;	
   ;	        }
   ;	        while (bloqueadosRaton.numElem > 0) ;
   ;	
	cmp	word ptr DGROUP:_bloqueadosRaton,0
	jg	short @20@4
@20@6:
   ;	
   ;	    }
   ;	}
   ;	
	pop	si
	leave	
	ret	
_procesarColaBloqueadosRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_actualizarCursor	proc	near
   ;	
   ;	void actualizarCursor ( void )
   ;	
   ;	
   ;	{
   ;	    if (cursorRatonActivo)               /* actualizar el cursor del raton */
   ;	
	cmp	word ptr DGROUP:_cursorRatonActivo,0
	jne	@@4
	jmp	@21@15
@@4:
   ;	
   ;	    {
   ;	        if (primeraInt) primeraInt = FALSE ;
   ;	
	cmp	word ptr DGROUP:_primeraInt,0
	je	short @21@3
	mov	word ptr DGROUP:_primeraInt,0
	jmp	@21@15
@21@3:
   ;	
   ;	        else if ((er.F != FAntCursor) || (er.C != CAntCursor))
   ;	
	mov	ax,word ptr DGROUP:_er+14
	cmp	ax,word ptr DGROUP:_FAntCursor
	jne	short @21@5
	mov	ax,word ptr DGROUP:_er+16
	cmp	ax,word ptr DGROUP:_CAntCursor
	jne	@@5
	jmp	@21@15
@@5:
@21@5:
   ;	
   ;	        {
   ;	            atrAux = ptrPant->t[FAntCursor][CAntCursor].atr ;
   ;	
	mov	bx,word ptr DGROUP:_FAntCursor
	imul	bx,bx,160
	mov	ax,word ptr DGROUP:_CAntCursor
	add	ax,ax
	add	bx,ax
	mov	ax,-18432
	mov	es,ax
	mov	al,byte ptr es:[bx+1]
	mov	byte ptr DGROUP:_atrAux,al
   ;	
   ;	//          atrAux = (atrAux << 4) | (atrAux >> 4) ; 
   ;	            if (atrAux != atrNormal)
   ;	
	cmp	byte ptr DGROUP:_atrAux,7
	je	short @21@10
   ;	
   ;	            {
   ;	                rotarDerecha(atrAux, 4) ;
   ;	
 	mov	 al,DGROUP:_atrAux 
 	ror	 al,4 
 	mov	 DGROUP:_atrAux,al 
   ;	
   ;	                ptrPant->t[FAntCursor][CAntCursor].atr = atrAux ;
   ;	
	mov	bx,word ptr DGROUP:_FAntCursor
	imul	bx,bx,160
	mov	ax,word ptr DGROUP:_CAntCursor
	add	ax,ax
	add	bx,ax
	mov	ax,-18432
	mov	dl,byte ptr DGROUP:_atrAux
	mov	es,ax
	mov	byte ptr es:[bx+1],dl
@21@10:
   ;	
   ;	            }
   ;	            atrAux = ptrPant->t[er.F][er.C].atr ;
   ;	
	mov	bx,word ptr DGROUP:_er+14
	imul	bx,bx,160
	mov	ax,word ptr DGROUP:_er+16
	add	ax,ax
	add	bx,ax
	mov	ax,-18432
	mov	es,ax
	mov	al,byte ptr es:[bx+1]
	mov	byte ptr DGROUP:_atrAux,al
   ;	
   ;	//          atrAux = (atrAux << 4) | (atrAux >> 4) ; 
   ;	            rotarDerecha(atrAux, 4) ;
   ;	
 	mov	 al,DGROUP:_atrAux 
 	ror	 al,4 
 	mov	 DGROUP:_atrAux,al 
   ;	
   ;	            ptrPant->t[er.F][er.C].atr = atrAux ;
   ;	
	mov	bx,word ptr DGROUP:_er+14
	imul	bx,bx,160
	mov	ax,word ptr DGROUP:_er+16
	add	ax,ax
	add	bx,ax
	mov	ax,-18432
	mov	dl,byte ptr DGROUP:_atrAux
	mov	es,ax
	mov	byte ptr es:[bx+1],dl
   ;	
   ;	            FAntCursor = er.F ;
   ;	
	mov	ax,word ptr DGROUP:_er+14
	mov	word ptr DGROUP:_FAntCursor,ax
   ;	
   ;	            CAntCursor = er.C ;
   ;	
	mov	ax,word ptr DGROUP:_er+16
	mov	word ptr DGROUP:_CAntCursor,ax
@21@15:
   ;	
   ;	        }
   ;	    }
   ;	}
   ;	
	ret	
_actualizarCursor	endp
	assume	cs:_TEXT,ds:DGROUP
_handlerRatonNulo	proc	far
   ;	
   ;	void far handlerRatonNulo ( void )
   ;	
   ;	
   ;	{
   ;	}
   ;	
	ret	
_handlerRatonNulo	endp
_TEXT	ends
_DATA	segment word public 'DATA'
$adacdaia	label	word
	db	1
	db	0
$igacdaia	label	word
	db	0
	db	0
$akacdaia	label	word
	db	0
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_handlerRaton	proc	far
   ;	
   ;	void far handlerRaton ( dword_t y, word_t x, word_t s )
   ;	
	enter	4,0
   ;	
   ;	{
   ;	    static bool_t priVez = TRUE ;       /* para ignorar los incrementos .. */
   ;	    static int incX = 0, incY = 0 ;     /* .. X,Y iniciales que son basura */
   ;	    int X1 ;
   ;	    int Y1 ;
   ;	
   ;		setraw_DS() ;                         /* establecemos el DS del driver */
   ;	
 	mov	 ax,word ptr cs:[3] 
 	mov	 dx,cs 
 	add	 ax,dx 
 	mov	 ds,ax 
   ;	
   ;		  
   ;	    if (priVez)
   ;	
	cmp	word ptr DGROUP:$adacdaia,0
	je	short @23@7
   ;	
   ;	        priVez = FALSE ;       /* Ignoro la primera lectura por ser basura */
   ;	
	mov	word ptr DGROUP:$adacdaia,0
	jmp	short @23@8
@23@7:
   ;	
   ;	    else
   ;	    {
   ;	        incX = x ;
   ;	
	mov	ax,word ptr [bp+10]
	mov	word ptr DGROUP:$igacdaia,ax
   ;	
   ;	        incY = (word_t)(y >> 16) ; /* Tienen sentido respecto a MOVING/SIZING y */
   ;	
	mov	dx,word ptr [bp+8]
	mov	word ptr DGROUP:$akacdaia,dx
   ;	
   ;	        er.B0 = s ;             /* (s & BITS_IGN) | (M.stat & ~BITS_IGN) ; */
   ;	
	mov	al,byte ptr [bp+12]
	mov	byte ptr DGROUP:_er+1,al
@23@8:
   ;	
   ;	    }                                             /* Respeto los que habia */
   ;	//                                         /* Considero el bit de signo .. */
   ;	    if (s & 0x20) incY |= 0xFF00 ;           /* ..haciendo una extension.. */
   ;	
	test	word ptr [bp+12],32
	je	short @23@10
	or	word ptr DGROUP:$akacdaia,-256
@23@10:
   ;	
   ;	    if (s & 0x10) incX |= 0xFF00 ;                   /* ..del bit de signo */
   ;	
	test	word ptr [bp+12],16
	je	short @23@12
	or	word ptr DGROUP:$igacdaia,-256
@23@12:
   ;	
   ;	    X1 = (er.X + incX) ;
   ;	
	mov	ax,word ptr DGROUP:_er+10
	add	ax,word ptr DGROUP:$igacdaia
	mov	word ptr [bp-2],ax
   ;	
   ;	    Y1 = (er.Y - incY) ;
   ;	
	mov	ax,word ptr DGROUP:_er+12
	sub	ax,word ptr DGROUP:$akacdaia
	mov	word ptr [bp-4],ax
   ;	
   ;	
   ;	    if (X1 < 0)             er.X = 0 ;
   ;	
	cmp	word ptr [bp-2],0
	jge	short @23@14
	mov	word ptr DGROUP:_er+10,0
	jmp	short @23@17
@23@14:
   ;	
   ;	    else if (X1 >= maxX)    er.X = maxX-1 ;
   ;	
	cmp	word ptr [bp-2],640
	jl	short @23@16
	mov	word ptr DGROUP:_er+10,639
	jmp	short @23@17
@23@16:
   ;	
   ;	    else                    er.X = X1 ;
   ;	
	mov	ax,word ptr [bp-2]
	mov	word ptr DGROUP:_er+10,ax
@23@17:
   ;	
   ;	
   ;	    if (Y1 < 0)             er.Y = 0 ;
   ;	
	cmp	word ptr [bp-4],0
	jge	short @23@19
	mov	word ptr DGROUP:_er+12,0
	jmp	short @23@22
@23@19:
   ;	
   ;	    else if (Y1 >= maxYAct) er.Y = maxYAct-1 ;
   ;	
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	shl	ax,3
	cmp	ax,word ptr [bp-4]
	jg	short @23@21
	xor	ax,ax
	mov	es,ax
	mov	al,byte ptr es:[1156]
	mov	ah,0
	inc	ax
	shl	ax,3
	dec	ax
	mov	word ptr DGROUP:_er+12,ax
	jmp	short @23@22
@23@21:
   ;	
   ;	    else                    er.Y = Y1 ;
   ;	
	mov	ax,word ptr [bp-4]
	mov	word ptr DGROUP:_er+12,ax
@23@22:
   ;	
   ;	}
   ;	
	leave	
	ret	
_handlerRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_finishRaton	proc	near
   ;	
   ;	int finishRaton ( void )
   ;	
   ;	
   ;	{
   ;	    exit(0) ;
   ;	
	push	0
	call	near ptr _exit
	pop	cx
   ;	
   ;	    return(0) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	ret	
_finishRaton	endp
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
_descCcbRaton	label	word
	db	0
	db	0
	db	0
	db	0
	db	0
	db	0
	db	9
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
	db	0
	db	0
	db	0
	db	0
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_outKBD	proc	near
   ;	
   ;	void outKBD ( byte_t puerto,   /* envia un comando al dispositivo auxiliar */
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	              byte_t valor )
   ;	{
   ;	asm 
   ;	{ 
   ;	    mov dl,puerto ;
   ;	
	mov	 dl,[bp+4] 
   ;	
   ;	    mov dh,valor ;
   ;	
	mov	 dh,[bp+6] 
   ;	
   ;	    push dx ;
   ;	
	push	 dx 
   ;	
   ;	    xor dx,dx ;
   ;	
	xor	 dx,dx 
   ;	
   ;	    mov es,dx ;
   ;	
	mov	 es,dx 
@26@6:
   ;	
   ;	outKBD_0: ;
   ;	
outKBD_0:
 	;	
   ;	
   ;	    mov dl,byte ptr es:[046Ch] ;                             /* BIOS_timer */
   ;	
	mov	 dl,byte ptr es:[046Ch] 
@26@9:
   ;	
   ;	outKBD_1: ;
   ;	
outKBD_1:
 	;	
   ;	
   ;	    in al,64h ;                                         /* keyboard status */
   ;	
	in	 al,64h 
   ;	
   ;	    test al,00000010b ;                    /* =2 check if we can send data */
   ;	
	test	 al,00000010b 
   ;	
   ;	    jmp outKBD_2 ;                           /* jump if write buffer empty */
   ;	
	jmp	short @26@18
   ;	
   ;	    cmp dl,byte ptr es:[046Ch] ;                             /* BIOS_timer */
   ;	
	cmp	 dl,byte ptr es:[046Ch] 
   ;	
   ;	    je outKBD_1 ;
   ;	
	je	short @26@9
   ;	
   ;	    xor dh,1 ;
   ;	
	xor	 dh,1 
   ;	
   ;	    jnz outKBD_0 ;
   ;	
	jne	short @26@6
@26@18:
   ;	
   ;	outKBD_2: ;
   ;	
outKBD_2:
 	;	
   ;	
   ;	    pop dx ;
   ;	
	pop	 dx 
   ;	
   ;	    mov al,dh ;
   ;	
	mov	 al,dh 
   ;	
   ;	    mov dh,0 ;
   ;	
	mov	 dh,0 
   ;	
   ;	    out dx,al ;
   ;	
	out	 dx,al 
   ;	
   ;	}
   ;	}
   ;	
	pop	bp
	ret	
_outKBD	endp
	assume	cs:_TEXT,ds:DGROUP
_flushKBD	proc	near
   ;	
   ;	void flushKBD ( byte_t puerto,         /* Send command to auxiliary device */
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	                byte_t valor )
   ;	{
   ;	    outKBD(puerto, valor) ;
   ;	
	mov	al,byte ptr [bp+6]
	push	ax
	mov	al,byte ptr [bp+4]
	push	ax
	call	near ptr _outKBD
	add	sp,4
   ;	
   ;	asm 
   ;	{
   ;	    mov ah,0 ;
   ;	
	mov	 ah,0 
@27@3:
   ;	
   ;	flushKBD_0: ;
   ;	
flushKBD_0:
 	;	
   ;	
   ;	    xor dx,dx ;
   ;	
	xor	 dx,dx 
   ;	
   ;	    mov es,dx ;
   ;	
	mov	 es,dx 
@27@7:
   ;	
   ;	flushKBD_1: ;
   ;	
flushKBD_1:
 	;	
   ;	
   ;	    mov dl,byte ptr es:[046Ch] ;                             /* BIOS_timer */
   ;	
	mov	 dl,byte ptr es:[046Ch] 
@27@10:
   ;	
   ;	flushKBD_2: ;
   ;	
flushKBD_2:
 	;	
   ;	
   ;	    in al,64h ;                                         /* keyboard status */
   ;	
	in	 al,64h 
   ;	
   ;	    test al,00000001b ;                          /* =1 if read buffer full */
   ;	
	test	 al,00000001b 
   ;	
   ;	    jz flushKBD_3 ;
   ;	
	je	short @27@18
   ;	
   ;	    in al,60h ;
   ;	
	in	 al,60h 
   ;	
   ;	    mov ah,al ;
   ;	
	mov	 ah,al 
   ;	
   ;	    jmp flushKBD_0 ;
   ;	
	jmp	short @27@3
@27@18:
   ;	
   ;	flushKBD_3: ;
   ;	
flushKBD_3:
 	;	
   ;	
   ;	    cmp dl,byte ptr es:[046Ch]                               /* BIOS_timer */
   ;	
	cmp	 dl,byte ptr es:[046Ch]                               
   ;	
   ;	    je flushKBD_2
   ;	
	je	short @27@10
   ;	
   ;	    xor dh,1
   ;	
	xor	 dh,1
   ;	
   ;	    jnz flushKBD_1
   ;	
	jne	short @27@7
   ;	
   ;	}	
   ;	}
   ;	
	pop	bp
	ret	
_flushKBD	endp
	assume	cs:_TEXT,ds:DGROUP
_setRate	proc	near
   ;	
   ;	void setRate ( byte_t rate )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	
   ;	    outKBD(0x64, 0x0D4) ;                        /* enable mouse functions */
   ;	
	push	212
	push	100
	call	near ptr _outKBD
	add	sp,4
   ;	
   ;	    flushKBD(0x60, 0x0F3) ;                      /* set rate...            */
   ;	
	push	243
	push	96
	call	near ptr _flushKBD
	add	sp,4
   ;	
   ;	//                                /* should be 0FAh (ACK) or 0FEh (resend) */
   ;	    outKBD(0x64, 0x0D4) ;                        /* enable mouse functions */
   ;	
	push	212
	push	100
	call	near ptr _outKBD
	add	sp,4
   ;	
   ;	    flushKBD(0x60, rate) ;                       /* ...value               */
   ;	
	mov	al,byte ptr [bp+4]
	push	ax
	push	96
	call	near ptr _flushKBD
	add	sp,4
   ;	
   ;	//                                /* should be 0FAh (ACK) or 0FEh (resend) */
   ;	}
   ;	
	pop	bp
	ret	
_setRate	endp
	assume	cs:_TEXT,ds:DGROUP
_PS2dummy	proc	far
   ;	
   ;	void far PS2dummy ( void )
   ;	
   ;	
   ;	{
   ;	}
   ;	
	ret	
_PS2dummy	endp
	assume	cs:_TEXT,ds:DGROUP
_checkPS2	proc	near
   ;	
   ;	bool_t checkPS2 ( printCar_t pCar )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	asm
   ;	{
   ;	    int 11h ;                                        /* get equipment list */
   ;	
	int	 11h 
   ;	
   ;	    and al,00000100b ;                                  /* 00000100 HW_PS2 */
   ;	
	and	 al,00000100b 
   ;	
   ;	    jz noPS2 ;                               /* jump if PS/2 not indicated */
   ;	
	je	short @30@33
   ;	
   ;	
   ;	    mov bh,3 ;
   ;	
	mov	 bh,3 
   ;	
   ;	    mov ax,0C205h ;                    /* inicializar raton, bh = datasize */
   ;	
	mov	 ax,0C205h 
   ;	
   ;	    int 15h ;
   ;	
	int	 15h 
   ;	
   ;	    jc noPS2 ;
   ;	
	jc	short @30@33
   ;	
   ;	    test ah,ah ;
   ;	
	test	 ah,ah 
   ;	
   ;	    jnz noPS2 ;
   ;	
	jne	short @30@33
   ;	
   ;	
   ;	    mov bh,3 ;
   ;	
	mov	 bh,3 
   ;	
   ;	    mov ax,0C203h ;             /* establecer la resoluci¢n del raton (bh) */
   ;	
	mov	 ax,0C203h 
   ;	
   ;	    int 15h ;
   ;	
	int	 15h 
   ;	
   ;	    jc noPS2 ;
   ;	
	jc	short @30@33
   ;	
   ;	    test ah,ah ;
   ;	
	test	 ah,ah 
   ;	
   ;	    jnz noPS2 ;
   ;	
	jne	short @30@33
   ;	
   ;	
   ;	    push cs ;
   ;	
	push	 cs 
   ;	
   ;	    pop es ;
   ;	
	pop	 es 
   ;	
   ;	    mov bx,OFFSET PS2dummy ;
   ;	
	mov	 bx,OFFSET _PS2dummy 
   ;	
   ;	    mov ax,0C207h ;                    /* establecer mouse handler (ES:BX) */
   ;	
	mov	 ax,0C207h 
   ;	
   ;	    int 15h ;
   ;	
	int	 15h 
   ;	
   ;	    jc noPS2 ;
   ;	
	jc	short @30@33
   ;	
   ;	    test ah,ah ;
   ;	
	test	 ah,ah 
   ;	
   ;	    jnz noPS2 ;
   ;	
	jne	short @30@33
   ;	
   ;	
   ;	    xor bx,bx ;
   ;	
	xor	 bx,bx 
   ;	
   ;	    mov es,bx ;
   ;	
	mov	 es,bx 
   ;	
   ;	    mov ax,0C207h ;                        /* borrar mouse handler (ES:BX) */
   ;	
	mov	 ax,0C207h 
   ;	
   ;	    int 15h ;
   ;	
	int	 15h 
   ;	
   ;	//                          /* select IntelliMouse Z wheel + 3 button mode */
   ;	    pushf ;
   ;	
	pushf	 
   ;	
   ;	    sti ;
   ;	
	sti	 
   ;	
   ;	}
   ;	    pCar('.') ;
   ;	
	push	46
	call	word ptr [bp+4]
	pop	cx
   ;	
   ;	    setRate(200) ;                          /* 200->100->80 rate does this */
   ;	
	push	200
	call	near ptr _setRate
	pop	cx
   ;	
   ;	    pCar('.') ;
   ;	
	push	46
	call	word ptr [bp+4]
	pop	cx
   ;	
   ;	    setRate(100) ;
   ;	
	push	100
	call	near ptr _setRate
	pop	cx
   ;	
   ;	    pCar('.') ;
   ;	
	push	46
	call	word ptr [bp+4]
	pop	cx
   ;	
   ;	    setRate( 80) ;
   ;	
	push	80
	call	near ptr _setRate
	pop	cx
   ;	
   ;	//                                                   /* check if sucessful */
   ;	    outKBD(0x64, 0x0D4) ;                        /* enable mouse functions */
   ;	
	push	212
	push	100
	call	near ptr _outKBD
	add	sp,4
   ;	
   ;	    flushKBD(0x60, 0x0F2) ;                      /* get ID en ah           */
   ;	
	push	242
	push	96
	call	near ptr _flushKBD
	add	sp,4
   ;	
   ;	//                                               /* ID=3 -> 3 button+wheel */
   ;	    asm popf
   ;	
 	popf	
   ;	
   ;	
   ;	    return(TRUE) ;                                          /* =PS/2 mouse */
   ;	
	mov	ax,1
	pop	bp
	ret	
@30@33:
   ;	
   ;	noPS2:
   ;	    return(FALSE) ;
   ;	
	xor	ax,ax
   ;	
   ;	}
   ;	
	pop	bp
	ret	
_checkPS2	endp
	assume	cs:_TEXT,ds:DGROUP
_disablePS2	proc	near
   ;	
   ;	void disablePS2 ( void )
   ;	
   ;	
   ;	{
   ;	    asm pushf ;
   ;	
 	pushf	 
   ;	
   ;	    asm cli ;
   ;	
 	cli	 
   ;	
   ;	    ptrTVI[nVIntRaton] = rtiRatonOrg ;  /* falta actualizar la mascara de interrupcion del pic ?? */
   ;	
	les	bx,dword ptr DGROUP:_ptrTVI
	mov	dx,word ptr DGROUP:_rtiRatonOrg+2
	mov	ax,word ptr DGROUP:_rtiRatonOrg
	mov	word ptr es:[bx+466],dx
	mov	word ptr es:[bx+464],ax
   ;	
   ;	    asm popf ;
   ;	
 	popf	 
   ;	
   ;	//                                         /* restablecerInt(nVIntRaton) ; */
   ;	asm
   ;	{
   ;	    xor bx,bx ;
   ;	
	xor	 bx,bx 
   ;	
   ;	    mov ax,0C200h ;                                       /* set mouse off */
   ;	
	mov	 ax,0C200h 
   ;	
   ;	    int 15h ;
   ;	
	int	 15h 
   ;	
   ;	
   ;	    mov es,bx ;
   ;	
	mov	 es,bx 
   ;	
   ;	    mov ax,0C207h ;                      /* borrar mouse handler (ES:BX=0) */
   ;	
	mov	 ax,0C207h 
   ;	
   ;	    int 15h ;
   ;	
	int	 15h 
   ;	
   ;	}
   ;	}
   ;	
	ret	
_disablePS2	endp
_TEXT	ends
_BSS	segment word public 'BSS'
$aedcdaia	label	word
	db	2 dup (?)
_BSS	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_enablePS2	proc	near
   ;	
   ;	void enablePS2 ( handler_t handler )
   ;	
	push	bp
	mov	bp,sp
   ;	
   ;	{
   ;	    static word_t handlerWord ;
   ;	
   ;	    handlerWord = FP_OFF(handler) ;
   ;	
	mov	ax,word ptr [bp+4]
	mov	word ptr DGROUP:$aedcdaia,ax
   ;	
   ;	asm 
   ;	{
   ;	    mov bx,0 ;
   ;	
	mov	 bx,0 
   ;	
   ;	    mov ax,0C200h ;                                       /* set mouse off */
   ;	
	mov	 ax,0C200h 
   ;	
   ;	    int 15h ;
   ;	
	int	 15h 
   ;	
   ;	
   ;	    mov bh,3 ;
   ;	
	mov	 bh,3 
   ;	
   ;	    mov ax,0C205h ;                    /* inicializar raton, bh = datasize */
   ;	
	mov	 ax,0C205h 
   ;	
   ;	    int 15h ;
   ;	
	int	 15h 
   ;	
   ;	
   ;	    mov bh,3 ;
   ;	
	mov	 bh,3 
   ;	
   ;	    mov ax,0C203h ;             /* establecer la resolucion del raton (bh) */
   ;	
	mov	 ax,0C203h 
   ;	
   ;	    int 15h ;
   ;	
	int	 15h 
   ;	
   ;	
   ;	#if (FALSE)
   ;	    mov bx,0 ;
   ;	    mov es,bx ;
   ;	    mov ax,0C207h ;                      /* borrar mouse handler (ES:BX=0) */
   ;	    int 15h ;
   ;	#endif
   ;	
   ;	    push cs ;
   ;	
	push	 cs 
   ;	
   ;	    pop es ;
   ;	
	pop	 es 
   ;	
   ;	    mov bx,handlerWord ;           /* necesitamos handlerWordl static (DS) */
   ;	
	mov	 bx,$aedcdaia 
   ;	
   ;	    mov ax,0C207h ;
   ;	
	mov	 ax,0C207h 
   ;	
   ;	    int 15h ;               /* Establece mouse handler ES:BX=handler_Raton */
   ;	
	int	 15h 
   ;	
   ;	
   ;	    mov bh,1 ;
   ;	
	mov	 bh,1 
   ;	
   ;	    mov ax,0C200h ;                                        /* set mouse on */
   ;	
	mov	 ax,0C200h 
   ;	
   ;	    int 15h ;
   ;	
	int	 15h 
   ;	
   ;	}
   ;	}
   ;	
	pop	bp
	ret	
_enablePS2	endp
	assume	cs:_TEXT,ds:DGROUP
_noPrintCar	proc	near
   ;	
   ;	void noPrintCar ( )                          /* no se utiliza el argumento */
   ;	
   ;	
   ;	{
   ;	}
   ;	
	ret	
_noPrintCar	endp
	assume	cs:_TEXT,ds:DGROUP
_inicRaton	proc	near
   ;	
   ;	void inicRaton ( tipoRaton_t * tipoRaton,
   ;	
	enter	20,0
   ;	
   ;	                 isr_t       * isrRaton,
   ;	                 bool_t        conMensajes)
   ;	{
   ;	    word_t numBotones ;
   ;	
   ;	    info_t info ;
   ;	
   ;	    bool_t hayNT ;
   ;	    bool_t hayDBox ;
   ;	
   ;	    printCar_t pCar ;
   ;	
   ;	    obtenInfoINFO((info_t far *)&info) ;
   ;	
	push	ss
	lea	ax,word ptr [bp-20]
	push	ax
	call	near ptr _obtenInfoINFO
	add	sp,4
   ;	
   ;	
   ;	    hayNT = FALSE ;
   ;	
	mov	word ptr [bp-4],0
   ;	
   ;	    switch (info.modoSO1)
   ;	
	mov	ax,word ptr [bp-14]
	cmp	ax,2
	je	short @34@6
	cmp	ax,3
	je	short @34@6
	jmp	short @34@7
@34@6:
   ;	
   ;	    {
   ;	    case modoSO1_Com :                                          /* so1.com */
   ;	    case modoSO1_Exe :                                          /* so1.exe */
   ;	        hayNT = (bool_t)(hayWindowsNT()) ;
   ;	
	call	near ptr _hayWindowsNT
	mov	word ptr [bp-4],ax
@34@7:
   ;	
   ;	    default :
   ;	        ;
   ;	    } ;
   ;	
   ;	    hayDBox = (bool_t)(!strncmp((char far *)ptrFechaBios, "01/01/92", 8)) ;
   ;	
	push	8
	push	ds
	push	offset DGROUP:s@
	push	-1
	push	5
	call	near ptr _strncmp
	add	sp,10
	neg	ax
	sbb	ax,ax
	inc	ax
	mov	word ptr [bp-6],ax
   ;	
   ;	
   ;	    if (conMensajes)
   ;	
	cmp	word ptr [bp+8],0
	je	short @34@12
   ;	
   ;	    {
   ;	        if (hayNT) printf("\n\n hayNT ") ;
   ;	
	cmp	word ptr [bp-4],0
	je	short @34@10
	push	ds
	push	offset DGROUP:s@+9
	call	near ptr _printf
	add	sp,4
@34@10:
   ;	
   ;	        if (hayDBox) printf("\n\n hayDBox ") ;
   ;	
	cmp	word ptr [bp-6],0
	je	short @34@12
	push	ds
	push	offset DGROUP:s@+19
	call	near ptr _printf
	add	sp,4
@34@12:
   ;	
   ;	    }
   ;	
   ;	    *tipoRaton = ninguno ;
   ;	
	mov	bx,word ptr [bp+4]
	mov	word ptr [bx],0
   ;	
   ;	
   ;	    inicEstadoRaton() ;
   ;	
	call	near ptr _inicEstadoRaton
   ;	
   ;	    if (hayNT)
   ;	
	cmp	word ptr [bp-4],0
	je	short @34@17
   ;	
   ;	    {
   ;	        resetRatonBIOS((word_t *)&numBotones) ;
   ;	
	lea	ax,word ptr [bp-2]
	push	ax
	call	near ptr _resetRatonBIOS
	pop	cx
   ;	
   ;	        if (hayRatonBIOS())                         /* raton msdos int 33h */
   ;	
	call	near ptr _hayRatonBIOS
	or	ax,ax
	je	short @34@15
   ;	
   ;	        {
   ;	            *tipoRaton = msdos ;                         /* WinXP o DOSBox */
   ;	
	mov	bx,word ptr [bp+4]
	mov	word ptr [bx],1
   ;	
   ;	            *isrRaton = isr_raton_BIOS ;
   ;	
	mov	bx,word ptr [bp+6]
	mov	word ptr [bx+2],seg _isr_raton_BIOS
	mov	word ptr [bx],offset _isr_raton_BIOS
   ;	
   ;	            installMouseEventHandler(     
   ;	
   ;	
   ;	                (handler_t)MK_FP(_CS, FP_OFF(ratonHandler))     
   ;	            ) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _ratonHandler
	push	ax
	push	dx
	call	near ptr _installMouseEventHandler
	add	sp,4
   ;	
   ;	        }
   ;	
	jmp	short @34@17
@34@15:
   ;	
   ;	        else if (conMensajes)
   ;	
	cmp	word ptr [bp+8],0
	je	short @34@17
   ;	
   ;	            printf("\n\n no hay raton BIOS ") ;
   ;	
	push	ds
	push	offset DGROUP:s@+31
	call	near ptr _printf
	add	sp,4
@34@17:
   ;	
   ;	    }
   ;	    if (*tipoRaton == ninguno)
   ;	
	mov	bx,word ptr [bp+4]
	cmp	word ptr [bx],0
	je	@@6
	jmp	@34@33
@@6:
   ;	
   ;	    {
   ;	        if (conMensajes)
   ;	
	cmp	word ptr [bp+8],0
	je	short @34@20
   ;	
   ;	            printf("\n\n comprobando si hay raton PS/2 ") ;
   ;	
	push	ds
	push	offset DGROUP:s@+53
	call	near ptr _printf
	add	sp,4
@34@20:
   ;	
   ;	        if (conMensajes)
   ;	
	cmp	word ptr [bp+8],0
	je	short @34@22
   ;	
   ;	            pCar = escribirCar ;
   ;	
	mov	word ptr [bp-8],offset _escribirCar
	jmp	short @34@23
@34@22:
   ;	
   ;	        else
   ;	            pCar = (printCar_t)noPrintCar ;
   ;	
	mov	word ptr [bp-8],offset _noPrintCar
@34@23:
   ;	
   ;	        if (checkPS2(pCar))
   ;	
	push	word ptr [bp-8]
	call	near ptr _checkPS2
	pop	cx
	or	ax,ax
	je	short @34@30
   ;	
   ;	        {
   ;	            if (conMensajes)
   ;	
	cmp	word ptr [bp+8],0
	je	short @34@26
   ;	
   ;	                printf(" Ok") ;
   ;	
	push	ds
	push	offset DGROUP:s@+87
	call	near ptr _printf
	add	sp,4
@34@26:
   ;	
   ;	            *tipoRaton = ps2 ;                                /* raton PS2 */
   ;	
	mov	bx,word ptr [bp+4]
	mov	word ptr [bx],2
   ;	
   ;	            if (hayDBox)                           /* dosbox (qemu, bochs) */
   ;	
	cmp	word ptr [bp-6],0
	je	short @34@28
   ;	
   ;	            {
   ;	                enablePS2((handler_t)handlerRaton) ;
   ;	
	push	seg _handlerRaton
	push	offset _handlerRaton
	call	near ptr _enablePS2
	add	sp,4
   ;	
   ;	                *isrRaton = isr_raton_dosbox ;
   ;	
	mov	bx,word ptr [bp+6]
	mov	word ptr [bx+2],seg _isr_raton_dosbox
	mov	word ptr [bx],offset _isr_raton_dosbox
   ;	
   ;	            }
   ;	
	jmp	short @34@29
@34@28:
   ;	
   ;	            else                                            /* qemu, bochs */
   ;	            {
   ;	                enablePS2((handler_t)handlerRatonNulo) ;
   ;	
	push	seg _handlerRatonNulo
	push	offset _handlerRatonNulo
	call	near ptr _enablePS2
	add	sp,4
   ;	
   ;	                *isrRaton = isr_raton ;
   ;	
	mov	bx,word ptr [bp+6]
	mov	word ptr [bx+2],seg _isr_raton
	mov	word ptr [bx],offset _isr_raton
@34@29:
   ;	
   ;	            }
   ;	        }
   ;	
	jmp	short @34@33
@34@30:
   ;	
   ;	        else
   ;	        {
   ;	            if (conMensajes)
   ;	
	cmp	word ptr [bp+8],0
	je	short @34@32
   ;	
   ;	                printf("\r no se ha encontrado ningun raton \n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+91
	call	near ptr _printf
	add	sp,4
@34@32:
   ;	
   ;	            exit(-1) ;
   ;	
	push	-1
	call	near ptr _exit
	pop	cx
@34@33:
   ;	
   ;	        }
   ;	    }
   ;	
   ;	    inicPC2c(&bloqueadosRaton, &e2BloqueadosRaton, maxProcesos + 0, FALSE) ;
   ;	
	push	0
	push	16
	push	ds
	push	offset DGROUP:_e2BloqueadosRaton
	push	ds
	push	offset DGROUP:_bloqueadosRaton
	call	near ptr _inicializarPC2c
	add	sp,12
   ;	
   ;	}
   ;	
	leave	
	ret	
_inicRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_mostrarFormato	proc	near
   ;	
   ;	void mostrarFormato ( void )
   ;	
   ;	
   ;	{
   ;	    printf(" formato: RATON [ [ -i | -q ] [ num ] | -u | -h ] ") ;
   ;	
	push	ds
	push	offset DGROUP:s@+128
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
	push	offset DGROUP:s@+179
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
	push	offset DGROUP:s@+182
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
	push	offset DGROUP:s@+184
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
   ;	        " instala/desinstala el driver del raton"                        "\n"
   ;	        ""                                                               "\n"
   ;	        " opciones: (por defecto -i)"                                    "\n"
   ;	        ""                                                               "\n"
   ;	        "      -i  : instala el driver (usar &)"                         "\n"
   ;	        "      -q  : instala sin mensajes de salida (&)"                 "\n"
   ;	        "      -u  : desintala el driver"                                "\n"
   ;	        "      -h  : muestra este help"                                  "\n"
   ;	    ) ;
   ;	
	push	ds
	push	offset DGROUP:s@+187
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
_TEXT	ends
_DATA	segment word public 'DATA'
_strTipoRaton	label	byte
	db	110
	db	105
	db	110
	db	103
	db	117
	db	110
	db	111
	db	1 dup (0)
	db	109
	db	115
	db	100
	db	111
	db	115
	db	3 dup (0)
	db	112
	db	115
	db	50
	db	5 dup (0)
_DATA	ends
_TEXT	segment byte public 'CODE'
	assume	cs:_TEXT,ds:DGROUP
_integrarRaton	proc	near
   ;	
   ;	int integrarRaton ( bool_t conMensajes )
   ;	
	enter	94,0
   ;	
   ;	{
   ;	    int dfRaton ;
   ;	    dfs_t dfs ;
   ;	    descRecurso_t dR ;
   ;		
   ;	    int res ;                                                 /* resultado */
   ;	    pindx_t pindx ;
   ;		
   ;	    isr_t isrRaton ;                                 /* rutina de servicio */
   ;		
   ;	    rti_t rtiRecRatonOrg ;     /* rutina de tratamiento de la interrupcion */
   ;	
   ;	    rtiRatonOrg = ptrTVI[nVIntRaton] ;     /* antes de instalar el handler */
   ;	
	les	bx,dword ptr DGROUP:_ptrTVI
	mov	dx,word ptr es:[bx+466]
	mov	ax,word ptr es:[bx+464]
	mov	word ptr DGROUP:_rtiRatonOrg+2,dx
	mov	word ptr DGROUP:_rtiRatonOrg,ax
   ;	
   ;	
   ;	#if (FALSE)                                             /* ya inicilizados */
   ;	    for ( pindx = 0 ; pindx < maxProcesos ; pindx++ )
   ;	    {
   ;	        nbytesProceso[pindx] = 0 ;          /* nbytes esperando ser leidos */
   ;	        dirProceso[pindx] = NULL ;               /* direcciones de destino */
   ;	    }	
   ;	#endif
   ;	
   ;	    inicRaton((tipoRaton_t *)&tipoRaton,
   ;	
   ;	
   ;	              (isr_t *)&isrRaton,
   ;	              (bool_t)conMensajes) ;               /* tipoRaton != ninguno */
   ;	
	push	word ptr [bp+4]
	lea	ax,word ptr [bp-6]
	push	ax
	push	offset DGROUP:_tipoRaton
	call	near ptr _inicRaton
	add	sp,6
   ;	
   ;	
   ;	    dR.tipo = rDCaracteres ;
   ;	
	mov	word ptr [bp-82],1
   ;	
   ;	    strcpy(dR.nombre, "RATON") ;
   ;	
	push	ds
	push	offset DGROUP:s@+408
	push	ss
	lea	ax,word ptr [bp-94]
	push	ax
	call	near ptr _strcpy
	add	sp,8
   ;	
   ;	    dR.ccb = (ccb_t)&descCcbRaton ;
   ;	
	mov	word ptr [bp-78],ds
	mov	word ptr [bp-80],offset DGROUP:_descCcbRaton
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
   ;	    dR.nVInt[0] = nVIntRaton ;
   ;	
	mov	byte ptr [bp-63],116
   ;	
   ;	    dR.irq[0] = IRQ_RATON ;
   ;	
	mov	byte ptr [bp-61],12
   ;	
   ;	    dR.isr[0] = (isr_t)MK_FP(_CS, FP_OFF(isrRaton)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,word ptr [bp-6]
	mov	word ptr [bp-57],ax
	mov	word ptr [bp-59],dx
   ;	
   ;	
   ;	    dR.open      = (open_t)     MK_FP(_CS, FP_OFF(openRaton)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _openRaton
	mov	word ptr [bp-49],ax
	mov	word ptr [bp-51],dx
   ;	
   ;	    dR.release   = (release_t)  MK_FP(_CS, FP_OFF(releaseRaton)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _releaseRaton
	mov	word ptr [bp-45],ax
	mov	word ptr [bp-47],dx
   ;	
   ;	    dR.read      = (read_t)     MK_FP(_CS, FP_OFF(readRaton)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _readRaton
	mov	word ptr [bp-41],ax
	mov	word ptr [bp-43],dx
   ;	
   ;	    dR.aio_read  = (aio_read_t) MK_FP(_CS, FP_OFF(aio_readRaton)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _aio_readRaton
	mov	word ptr [bp-37],ax
	mov	word ptr [bp-39],dx
   ;	
   ;	    dR.write     = (write_t)    MK_FP(_CS, FP_OFF(writeRaton)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _writeRaton
	mov	word ptr [bp-33],ax
	mov	word ptr [bp-35],dx
   ;	
   ;	    dR.aio_write = (aio_write_t)MK_FP(_CS, FP_OFF(aio_writeRaton)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _aio_writeRaton
	mov	word ptr [bp-29],ax
	mov	word ptr [bp-31],dx
   ;	
   ;	    dR.lseek     = (lseek_t)    MK_FP(_CS, FP_OFF(lseekRaton)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _lseekRaton
	mov	word ptr [bp-25],ax
	mov	word ptr [bp-27],dx
   ;	
   ;	    dR.fcntl     = (fcntl_t)    MK_FP(_CS, FP_OFF(fcntlRaton)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _fcntlRaton
	mov	word ptr [bp-21],ax
	mov	word ptr [bp-23],dx
   ;	
   ;	    dR.ioctl     = (ioctl_t)    MK_FP(_CS, FP_OFF(ioctlRaton)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _ioctlRaton
	mov	word ptr [bp-17],ax
	mov	word ptr [bp-19],dx
   ;	
   ;	
   ;	    dR.eliminar  = (eliminar_t) MK_FP(_CS, FP_OFF(eliminarRaton)) ;
   ;	
	mov	ax,cs
	xor	dx,dx
	or	dx,offset _eliminarRaton
	mov	word ptr [bp-13],ax
	mov	word ptr [bp-15],dx
   ;	
   ;	
   ;	    asm pushf
   ;	
 	pushf	
   ;	
   ;	    asm cli
   ;	
 	cli	
   ;	
   ;	
   ;	    rtiRecRatonOrg = ptrTVI[nVIntRaton] ;      /* tras instalar el handler */
   ;	
	les	bx,dword ptr DGROUP:_ptrTVI
	mov	dx,word ptr es:[bx+466]
	mov	ax,word ptr es:[bx+464]
	mov	word ptr [bp-8],dx
	mov	word ptr [bp-10],ax
   ;	
   ;	    rec_raton = crearRecurso(&dR) ;
   ;	
	push	ss
	lea	ax,word ptr [bp-94]
	push	ax
	call	near ptr _crearRecurso
	add	sp,4
	mov	word ptr DGROUP:_rec_raton,ax
   ;	
   ;	    if (tipoRaton == msdos)
   ;	
	cmp	word ptr DGROUP:_tipoRaton,1
	jne	short @38@6
   ;	
   ;	    {
   ;	        rti_nVIntRaton = ptrTVI[nVIntRaton] ;        /* ver so1\interrup.c */
   ;	
	les	bx,dword ptr DGROUP:_ptrTVI
	mov	dx,word ptr es:[bx+466]
	mov	ax,word ptr es:[bx+464]
	mov	word ptr DGROUP:_rti_nVIntRaton+2,dx
	mov	word ptr DGROUP:_rti_nVIntRaton,ax
   ;	
   ;	        segment_rti_nVIntRaton = FP_SEG(rti_nVIntRaton) ;
   ;	
	mov	dx,word ptr DGROUP:_rti_nVIntRaton+2
	mov	word ptr DGROUP:_segment_rti_nVIntRaton,dx
   ;	
   ;	        offset_rti_nVIntRaton = FP_OFF(rti_nVIntRaton) ;
   ;	
	mov	ax,word ptr DGROUP:_rti_nVIntRaton
	mov	word ptr DGROUP:_offset_rti_nVIntRaton,ax
   ;	
   ;	        ptrTVI[nVIntRaton] = rtiRecRatonOrg ; /* restablecer el v.i.nVIntRaton */
   ;	
	les	bx,dword ptr DGROUP:_ptrTVI
	mov	dx,word ptr [bp-8]
	mov	ax,word ptr [bp-10]
	mov	word ptr es:[bx+466],dx
	mov	word ptr es:[bx+464],ax
@38@6:
   ;	
   ;	    }
   ;		
   ;	    asm popf
   ;	
 	popf	
   ;	
   ;	
   ;	    if (rec_raton >= 0)
   ;	
	cmp	word ptr DGROUP:_rec_raton,0
	jge	@@7
	jmp	@38@22
@@7:
   ;	
   ;	    {
   ;	        dfs = crearFichero("RATON", rec_raton, 0, fedCaracteres) ;
   ;	
	push	3
	push	0
	push	word ptr DGROUP:_rec_raton
	push	ds
	push	offset DGROUP:s@+414
	call	near ptr _crearFichero
	add	sp,10
	mov	word ptr [bp-2],ax
   ;	
   ;	
   ;	        if (dfs < 0)
   ;	
	cmp	word ptr [bp-2],0
	jge	short @38@19
   ;	
   ;	        {
   ;	            switch(dfs)
   ;	
	mov	bx,word ptr [bp-2]
	sub	bx,-4
	cmp	bx,3
	ja	short @38@16
	add	bx,bx
	jmp	word ptr cs:@38@C92[bx]
@38@12:
   ;	
   ;	            {
   ;	            case -1 : printf(" tipo de fichero erroneo") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+420
	jmp	short @38@17
@38@13:
   ;	
   ;	            case -2 : printf(" numero de recurso erroneo") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+445
	jmp	short @38@17
@38@14:
   ;	
   ;	            case -3 : printf(" RATON nombre de fichero ya existente") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+472
	jmp	short @38@17
@38@15:
   ;	
   ;	            case -4 : printf(" no hay descriptores de fichero libres") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+510
	jmp	short @38@17
@38@16:
   ;	
   ;	            default : printf(" no ha podido crearse el fichero RATON") ;
   ;	
	push	ds
	push	offset DGROUP:s@+549
@38@17:
	call	near ptr _printf
	add	sp,4
   ;	
   ;	            }
   ;	            destruirRecurso("RATON") ;
   ;	
	push	ds
	push	offset DGROUP:s@+588
	call	near ptr _destruirRecurso
	add	sp,4
@38@19:
   ;	
   ;	        }
   ;	        if (conMensajes)
   ;	
	cmp	word ptr [bp+4],0
	je	short @38@21
   ;	
   ;	            printf(
   ;	
   ;	
   ;	                ""                                                       "\n"
   ;	                ""                                                       "\n"
   ;	                " recurso RATON instalado (fichero: RATON)"              "\n"
   ;	                ""                                                       "\n"
   ;	                " tipo de raton = %s\n", 
   ;					strTipoRaton[tipoRaton]
   ;	            ) ;
   ;	
	mov	ax,word ptr DGROUP:_tipoRaton
	shl	ax,3
	add	ax,offset DGROUP:_strTipoRaton
	push	ax
	push	ds
	push	offset DGROUP:s@+594
	call	near ptr _printf
	add	sp,6
@38@21:
   ;	
   ;	        return(0) ;
   ;	
	xor	ax,ax
	leave	
	ret	
@38@22:
   ;	
   ;	    }
   ;	
   ;	    printf("\n\n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+660
	call	near ptr _printf
	add	sp,4
   ;	
   ;	    switch(rec_raton)
   ;	
	mov	bx,word ptr DGROUP:_rec_raton
	sub	bx,-5
	cmp	bx,4
	ja	short @38@31
	add	bx,bx
	jmp	word ptr cs:@38@C97[bx]
@38@26:
   ;	
   ;	    {
   ;	    case -1 : printf(" tipo de recurso erroneo ") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+663
	jmp	short @38@32
@38@27:
   ;	
   ;	    case -2 : printf(" demasiados vectores de interrupcion ") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+689
	jmp	short @38@32
@38@28:
   ;	
   ;	    case -3 : printf(" RATON nombre de recurso ya existente ") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+727
	jmp	short @38@32
@38@29:
   ;	
   ;	    case -4 : printf(" no hay descriptores de recurso libres ") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+766
	jmp	short @38@32
@38@30:
   ;	
   ;	    case -5 : printf(" numero de vector de interrupcion ya usado ") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+806
	jmp	short @38@32
@38@31:
   ;	
   ;	    default : printf(" no ha podido crearse el recurso RATON ") ;
   ;	
	push	ds
	push	offset DGROUP:s@+850
@38@32:
	call	near ptr _printf
	add	sp,4
   ;	
   ;	    }
   ;	    printf("\n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+890
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
_integrarRaton	endp
@38@C97	label	word
	dw	@38@30
	dw	@38@29
	dw	@38@28
	dw	@38@27
	dw	@38@26
@38@C92	label	word
	dw	@38@15
	dw	@38@14
	dw	@38@13
	dw	@38@12
	assume	cs:_TEXT,ds:DGROUP
_desintegrarRaton	proc	near
   ;	
   ;	int desintegrarRaton ( void )
   ;	
	enter	2,0
   ;	
   ;	{
   ;	    int res ;
   ;	    res = 0 ;
   ;	
	mov	word ptr [bp-2],0
   ;	
   ;	    switch (tipoRaton)
   ;	
	mov	ax,word ptr DGROUP:_tipoRaton
	cmp	ax,1
	je	short @39@6
	cmp	ax,2
	je	short @39@7
	jmp	short @39@8
@39@6:
   ;	
   ;	    {
   ;	    case msdos :
   ;	        uninstallMouseEventHandler(ratonHandler) ;
   ;	
	push	seg _ratonHandler
	push	offset _ratonHandler
	call	near ptr _uninstallMouseEventHandler
	add	sp,4
   ;	
   ;	        break ;
   ;	
	jmp	short @39@8
@39@7:
   ;	
   ;	    case ps2   :
   ;	        disablePS2() ;
   ;	
	call	near ptr _disablePS2
@39@8:
   ;	
   ;	    }
   ;	    return(res) ;
   ;	
	mov	ax,word ptr [bp-2]
   ;	
   ;	}
   ;	
	leave	
	ret	
_desintegrarRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_instalarRaton	proc	near
   ;	
   ;	int instalarRaton ( bool_t conMensajes )
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
	je	short @40@3
	jmp	short @40@6
@40@3:
   ;	
   ;	    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
   ;	
	push	ds
	push	offset DGROUP:_ptrIndProcesoActual
	call	near ptr _obtenInfoSO1
	add	sp,4
   ;	
   ;	    res = integrarRaton(conMensajes) ;
   ;	
	push	word ptr [bp+4]
	call	near ptr _integrarRaton
	pop	cx
	mov	word ptr [bp-2],ax
   ;	
   ;	    if (res != 0) return(res) ;
   ;	
	cmp	word ptr [bp-2],0
	je	short @40@5
	jmp	short @40@6
@40@5:
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
   ;	        FP_OFF(&descCcbRaton)
   ;	            + sizeof(descCcbRaton) + 9*sizeof(callBack_t),      /* tamDATA */
   ;	        FP_OFF(finCodeDriver),                            /* finCodeDriver */
   ;	        FP_OFF(finishRaton)                                /* finishDriver */
   ;	    ) ;
   ;	
	push	offset _finishRaton
	push	offset _finCodeDriver
	mov	ax,offset DGROUP:_descCcbRaton
	add	ax,48
	push	ax
	call	near ptr _esperarDesinstalacion
	add	sp,6
   ;	
   ;	#endif
   ;	    res = desintegrarRaton() ;
   ;	
	call	near ptr _desintegrarRaton
	mov	word ptr [bp-2],ax
@40@6:
   ;	
   ;	    return(res) ;
   ;	
	mov	ax,word ptr [bp-2]
   ;	
   ;	}
   ;	
	leave	
	ret	
_instalarRaton	endp
	assume	cs:_TEXT,ds:DGROUP
_main	proc	near
   ;	
   ;	int main ( int argc, char * argv [ ] )
   ;	
	enter	2,0
   ;	
   ;	{
   ;	    int res ;
   ;	    if (argc > 3) return(formato()) ;
   ;	
	cmp	word ptr [bp+4],3
	jle	short @41@4
	jmp	@41@43
	jmp	@41@30
@41@4:
   ;	
   ;	    else if (argc == 1) return(instalarRaton(TRUE)) ;
   ;	
	cmp	word ptr [bp+4],1
	jne	short @41@7
	jmp	short @41@12
	jmp	@41@30
@41@7:
   ;	
   ;	    else if (argc == 2)
   ;	
	cmp	word ptr [bp+4],2
	je	@@8
	jmp	@41@30
@@8:
   ;	
   ;	    {
   ;	        if (!strcmpu(argv[1], "-h")) return(help()) ;
   ;	
	push	ds
	push	offset DGROUP:s@+892
	push	ds
	mov	bx,word ptr [bp+6]
	push	word ptr [bx+2]
	call	near ptr _strcmpu
	add	sp,8
	or	ax,ax
	jne	short @41@11
	call	near ptr _help
	leave	
	ret	
	jmp	@41@30
@41@11:
   ;	
   ;	        else if (!strcmpu(argv[1], "-i")) return(instalarRaton(TRUE)) ;
   ;	
	push	ds
	push	offset DGROUP:s@+895
	push	ds
	mov	bx,word ptr [bp+6]
	push	word ptr [bx+2]
	call	near ptr _strcmpu
	add	sp,8
	or	ax,ax
	jne	short @41@14
@41@12:
	push	1
	jmp	short @41@16
	jmp	@41@30
@41@14:
   ;	
   ;	        else if (!strcmpu(argv[1], "-q")) return(instalarRaton(FALSE)) ;
   ;	
	push	ds
	push	offset DGROUP:s@+898
	push	ds
	mov	bx,word ptr [bp+6]
	push	word ptr [bx+2]
	call	near ptr _strcmpu
	add	sp,8
	or	ax,ax
	jne	short @41@18
	push	0
@41@16:
	call	near ptr _instalarRaton
	pop	cx
	leave	
	ret	
	jmp	short @41@30
@41@18:
   ;	
   ;	        else if (!strcmpu(argv[1], "-u"))
   ;	
	push	ds
	push	offset DGROUP:s@+901
	push	ds
	mov	bx,word ptr [bp+6]
	push	word ptr [bx+2]
	call	near ptr _strcmpu
	add	sp,8
	or	ax,ax
	jne	short @41@30
   ;	
   ;	        {
   ;	            res = destruirRecurso("RATON") ;
   ;	
	push	ds
	push	offset DGROUP:s@+904
	call	near ptr _destruirRecurso
	add	sp,4
	mov	word ptr [bp-2],ax
   ;	
   ;	            switch (res)
   ;	
	mov	bx,word ptr [bp-2]
	sub	bx,-3
	cmp	bx,3
	ja	short @41@27
	add	bx,bx
	jmp	word ptr cs:@41@C97[bx]
@41@23:
   ;	
   ;	            {
   ;	            case  0 : printf(" recurso RATON desinstalado") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+910
	jmp	short @41@28
@41@24:
   ;	
   ;	            case -1 : printf(" recurso RATON no existe") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+938
	jmp	short @41@28
@41@25:
   ;	
   ;	            case -2 : printf(" recurso RATON en uso") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+963
	jmp	short @41@28
@41@26:
   ;	
   ;	            case -3 : printf(" fichero RATON no puede destruirse") ; break ;
   ;	
	push	ds
	push	offset DGROUP:s@+985
	jmp	short @41@28
@41@27:
   ;	
   ;	            default : printf(" RATON no ha podido desinstalarse") ;
   ;	
	push	ds
	push	offset DGROUP:s@+1020
@41@28:
	call	near ptr _printf
	add	sp,4
   ;	
   ;	            }
   ;	            return(res) ;
   ;	
	mov	ax,word ptr [bp-2]
	leave	
	ret	
@41@30:
   ;	
   ;	        }
   ;	    }
   ;	    if ((argc == 2) ||
   ;	
   ;	
   ;	            ((argc == 3) &&
   ;	             (!strcmpu(argv[1], "-i") ||
   ;	              !strcmpu(argv[1], "-q"))
   ;	            )
   ;	       )
   ;	
	cmp	word ptr [bp+4],2
	je	short @41@34
	cmp	word ptr [bp+4],3
	je	@@9
	jmp	@41@43
@@9:
	push	ds
	push	offset DGROUP:s@+1054
	push	ds
	mov	bx,word ptr [bp+6]
	push	word ptr [bx+2]
	call	near ptr _strcmpu
	add	sp,8
	or	ax,ax
	je	short @41@34
	push	ds
	push	offset DGROUP:s@+1057
	push	ds
	mov	bx,word ptr [bp+6]
	push	word ptr [bx+2]
	call	near ptr _strcmpu
	add	sp,8
	or	ax,ax
	jne	short @41@43
@41@34:
   ;	
   ;	    {
   ;	        strcpy(comando[0], argv[argc-1]) ;
   ;	
	push	ds
	mov	bx,word ptr [bp+4]
	dec	bx
	add	bx,bx
	add	bx,word ptr [bp+6]
	push	word ptr [bx]
	push	ds
	push	offset DGROUP:_comando
	call	near ptr _strcpy
	add	sp,8
   ;	
   ;	        inicScanner() ;
   ;	
	call	near ptr _inicScanner
   ;	
   ;	        obtenSimb() ;
   ;	
	call	near ptr _obtenSimb
   ;	
   ;	        if (simb == s_numero)
   ;	
	cmp	word ptr DGROUP:_simb,19
	jne	short @41@43
   ;	
   ;	        {
   ;	            if (num == 0)
   ;	
	cmp	word ptr DGROUP:_num,0
	jne	short @41@39
   ;	
   ;	            {
   ;	                printf("\n\n numConsolas debe ser > 0 \n") ;
   ;	
	push	ds
	push	offset DGROUP:s@+1060
	call	near ptr _printf
	add	sp,4
   ;	
   ;	                return(-1) ;
   ;	
	mov	ax,-1
	leave	
	ret	
   ;	
   ;	            }
   ;	
	jmp	short @41@43
@41@39:
   ;	
   ;	            else return(instalarRaton(toupper(argv[1][1]) == 'Q')) ;
   ;	
	mov	bx,word ptr [bp+6]
	mov	bx,word ptr [bx+2]
	mov	al,byte ptr [bx+1]
	cbw	
	push	ax
	call	near ptr _toupper
	pop	cx
	cmp	ax,81
	jne	short @41@41
	mov	ax,1
	jmp	short @41@42
@41@41:
	xor	ax,ax
@41@42:
	push	ax
	jmp	@41@16
@41@43:
   ;	
   ;	        }
   ;	    }
   ;	    return(formato()) ;
   ;	
	call	near ptr _formato
   ;	
   ;	}
   ;	
	leave	
	ret	
_main	endp
@41@C97	label	word
	dw	@41@26
	dw	@41@25
	dw	@41@24
	dw	@41@23
_TEXT	ends
_BSS	segment word public 'BSS'
_rtiRatonOrg	label	dword
	db	4 dup (?)
_tipoRaton	label	word
	db	2 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'01/01/92'
	db	0
	db	10
	db	10
	db	' hayNT '
	db	0
	db	10
	db	10
	db	' hayDBox '
	db	0
	db	10
	db	10
	db	' no hay raton BIOS '
	db	0
	db	10
	db	10
	db	' comprobando si hay raton PS/2 '
	db	0
	db	' Ok'
	db	0
	db	13
	db	' no se ha encontrado ningun raton '
	db	10
	db	0
	db	' formato: RATON [ [ -i | -q ] [ num ] | -u | -h ] '
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
	db	' instala/desinstala el driver del raton'
	db	10
	db	10
	db	' opciones: (por defecto -i)'
	db	10
	db	10
	db	'      -i  : instala el driver (usar &)'
	db	10
	db	'      -q  : instala sin mensajes de salida (&)'
	db	10
	db	'      -u  : desintala el driver'
	db	10
	db	'      -h  : muestra este help'
	db	10
	db	0
	db	'RATON'
	db	0
	db	'RATON'
	db	0
	db	' tipo de fichero erroneo'
	db	0
	db	' numero de recurso erroneo'
	db	0
	db	' RATON nombre de fichero ya existente'
	db	0
	db	' no hay descriptores de fichero libres'
	db	0
	db	' no ha podido crearse el fichero RATON'
	db	0
	db	'RATON'
	db	0
	db	10
	db	10
	db	' recurso RATON instalado (fichero: RATON)'
	db	10
	db	10
	db	' tipo de raton = %s'
	db	10
	db	0
	db	10
	db	10
	db	0
	db	' tipo de recurso erroneo '
	db	0
	db	' demasiados vectores de interrupcion '
	db	0
	db	' RATON nombre de recurso ya existente '
	db	0
	db	' no hay descriptores de recurso libres '
	db	0
	db	' numero de vector de interrupcion ya usado '
	db	0
	db	' no ha podido crearse el recurso RATON '
	db	0
	db	10
	db	0
	db	'-h'
	db	0
	db	'-i'
	db	0
	db	'-q'
	db	0
	db	'-u'
	db	0
	db	'RATON'
	db	0
	db	' recurso RATON desinstalado'
	db	0
	db	' recurso RATON no existe'
	db	0
	db	' recurso RATON en uso'
	db	0
	db	' fichero RATON no puede destruirse'
	db	0
	db	' RATON no ha podido desinstalarse'
	db	0
	db	'-i'
	db	0
	db	'-q'
	db	0
	db	10
	db	10
	db	' numConsolas debe ser > 0 '
	db	10
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	extrn	_eliminarPC2c:near
	extrn	_encolarPC2c:near
	extrn	_desencolarPC2c:near
	extrn	_inicializarPC2c:near
	extrn	_comando:byte
	extrn	_simb:word
	extrn	_num:word
	extrn	_inicScanner:near
	extrn	_obtenSimb:near
	extrn	_comprobarAmpersand:near
	extrn	_exit:near
	extrn	_getpindx:near
	extrn	_obtenInfoSO1:near
	extrn	_crearRecurso:near
	extrn	_crearFichero:near
	extrn	_esperarDesinstalacion:near
	extrn	_destruirRecurso:near
	extrn	_obtenInfoINFO:near
	extrn	_printf:near
	extrn	_escribirCar:near
	extrn	_toupper:near
	extrn	_strcpy:near
	extrn	_strcmpu:near
	extrn	_strncmp:near
	extrn	_memcpy:near
	extrn	_ptrTVI:dword
	extrn	_hayWindowsNT:near
	extrn	_hayRatonBIOS:near
	extrn	_resetRatonBIOS:near
	public	_rti_raton_BIOS
	extrn	_installMouseEventHandler:near
	extrn	_uninstallMouseEventHandler:near
	public	_ratonHandler
	public	_rec_raton
	public	_bloqueadosRaton
	public	_e2BloqueadosRaton
	public	_nbytesProceso
	public	_dirProceso
	public	_inB
	public	_B
	public	_er
	public	_XAux
	public	_YAux
	public	_FAntCursor
	public	_CAntCursor
	public	_atrAux
	public	_cursorRatonActivo
	public	_moviendo
	public	_redimensionando
	public	_primerClick
	public	_contTics2Click
	public	_huboInt
	public	_primeraInt
	public	_inicEstadoRaton
	public	_leerEstadoRaton
	public	_openRaton
	public	_releaseRaton
	public	_readRaton
	public	_aio_readRaton
	public	_writeRaton
	public	_aio_writeRaton
	public	_lseekRaton
	public	_fcntlRaton
	public	_ioctlRaton
	public	_eliminarRaton
	public	_procesarRaton
	public	_procesarColaBloqueadosRaton
	public	_tratarRaton
	public	_actualizarCursor
	public	_isr_raton
	public	_isr_raton_dosbox
	public	_rH_CS
	public	_rH_IP
	public	_rH_AX
	public	_rH_BX
	public	_rH_CX
	public	_rH_DX
	public	_rH_SI
	public	_rH_DI
	public	_isr_raton_BIOS
	public	_rti_nVIntRaton
	public	_segment_rti_nVIntRaton
	public	_offset_rti_nVIntRaton
	public	_ptrWordAux
	public	_handlerRatonNulo
	public	_handlerRaton
	public	_finishRaton
	public	_finCodeDriver
	public	_descCcbRaton
	public	_outKBD
	public	_flushKBD
	public	_setRate
	public	_PS2dummy
	public	_checkPS2
	public	_rtiRatonOrg
	public	_disablePS2
	public	_enablePS2
	public	_noPrintCar
	public	_inicRaton
	public	_mostrarFormato
	public	_formato
	public	_help
	public	_strTipoRaton
	public	_tipoRaton
	public	_integrarRaton
	public	_desintegrarRaton
	public	_instalarRaton
	public	_main
	extrn	__setargv__:far
	end
