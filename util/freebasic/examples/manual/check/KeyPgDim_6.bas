'' examples/manual/check/KeyPgDim_6.bas
''
'' NOTICE: This file is part of the FreeBASIC Compiler package and can't
''         be included in other distributions without authorization.
''
'' See Also: http://www.freebasic.net/wiki/wikka.php?wakka=KeyPgDim
'' --------

'' declare a simple UDT
Type mytype
	var1 As Double
	var2 As Integer
End Type

'' declare a 3 element array and initialize the first
'' 2 mytype elements
Dim myvar(0 To 2) As mytype => {(1.0, 1), (2.0, 2)}
