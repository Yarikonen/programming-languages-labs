%define next 0
%macro colon 2
	%ifid %2
		%2: dq next
		%define next %2
	%else
		%error "Second argument must be identifier to current el"
	%endif
	%ifstr %1
		db %1,0
	%else
		%error "First argument must be string"
	%endif
%endmacro