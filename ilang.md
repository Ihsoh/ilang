# ilang参考手册

## EBNF
```ebnf


ws = ? 一个到多个：空格（0x20）、水平制表符（0x09）、新行符（0x0a）、垂直制表符（0x0b）、换页符（0x0c）、回车符（0x0d） ?;

digit_bin = "0" | "1";
digit_oct = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7";
digit_dec = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
digit_hex = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
		| "a" | "b" | "c" | "d" | "e" | "f"
		| "A" | "B" | "C" | "D" | "E" | "F";

uint_bin = "0b", digit_bin, {digit_bin | "_"};
uint_oct = "0o", digit_oct, {digit_oct | "_"};
uint_dec = {digit_dec};
uint_hex = "0x", digit_hex, {digit_hex | "_"};

uint = uint_bin | uint_oct | uint_dec | uint_hex;
float = uint_dec, ".", uint_dec;

char = "'", (? 除单引号外的所有可见ASCII字符 ? | "\a" | "\b" | "\f" | "\n" | "\r" | "\t" | "\v" | "\\" | "\'"), "'";
string = '"', (? 除双引号外的所有可见ASCII字符 ? | "\a" | "\b" | "\f" | "\n" | "\r" | "\t" | "\v" | "\\" | '\"' ), '"';

identifier_char = "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n"
		| "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
		| "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N"
		| "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z"
		| "_";
identifier = identifier_char, {identifier_char | digit_dec};

type = primitive_type | struct_type | array_type | function_type | pointer_type;

primitive_type = "char"
		| "int8" | "int16" | "int32" | "int64"
		| "uint8" | "uint16" | "uint32" | "uint64"
		| "float" | "double";

struct_type = "struct", ws, identifier;

array_type = "[", [ws], uint, [ws], "]", {[ws], "[", [ws], uint, [ws], "]"}, [ws], type;

function_param = identifier, [ws], ":", [ws], type;
function_params = function_param, {[ws], ",", [ws], function_param}, [[ws], ",", [ws], "..."];
function_type = "function", [ws], "(", [ws], [function_params], [ws], ")", [[ws], ":", [ws], type];

pointer_type = "*" | ("*", [ws], type);

var_definition_item = identifier, [ws], ":", [ws], type, [ws], "=", [ws], expr;
var_definition = "var", ws, var_definition_item, {[ws], ",", [ws], var_definition_item};

struct_declaration = "struct", ws, identifier, ws, "dummy";

struct_definition_member_var_item = identifier, [ws], ":", [ws], type;
struct_definition_member_var = "var", ws, struct_definition_member_var_item,
		{[ws], ",", [ws], struct_definition_member_var_item}, [ws], ";";
struct_definition_body = {[ws], struct_definition_member_var, [ws]};
struct_definition = "struct", ws, identifier, [ws], "{", {struct_definition_body}, "}";

function_declaration = "function", ws, identifier, [ws], "(", [ws], [function_params], [ws], ")",
		(([[ws], ":", [ws], type, ws, "dummy"]) | ([ws], "dummy"));

function_definition = "function", ws, identifier, [ws], "(", [ws], [function_params], [ws], ")",
		[[ws], ":", [ws], type], [ws], statement_block;

stat_var = var_definition, [ws], ";";

stat_if_else = "else", [ws], statement_block;
stat_if_elif = "elif", [ws], "(", [ws], expr, [ws], ")", [ws], statement_block;
stat_if = "if", [ws], "(", [ws], expr, [ws], ")", [ws], statement_block,
		{[ws], stat_if_elif},
		[[ws], stat_if_else];

stat_while = "while", [ws], "(", [ws], expr, [ws], ")", [ws], statement_block;

stat_do = "do",
		[ws], statment_block,
		[ws], "while", [ws], "(", [ws], expr, [ws], ")", [ws], ";";

stat_for = "for", [ws], "(",
		[ws], var_definition, [ws], ";",
		[ws], expr, [ws], ";"
		[ws], expr, [ws],
		")", [ws], statement_block;

stat_break = "break", [ws], ";";

stat_continue = "continue", [ws], ";";

stat_return = ("return", [ws], ";") | ("return", [ws], expr, [ws], ";");

stat_goto = "goto", ws, identifier, [ws], ";";

stat_label = "label", ws, identifier, [ws], ";";
stat_label_simple = identifier, [ws], ":";

stat_asm_option = string, [ws], "(", [ws], expr, [ws], ")";
stat_asm = "asm", [ws], "(", [ws], string, {[ws], ",", [ws], stat_asm_option}, [ws], ")", [ws], ";";

stat_expr = expr, [ws], ";";

stat_dummy = ';';

statement = stat_var
		| stat_if
		| stat_while
		| stat_do
		| stat_for
		| stat_break
		| stat_continue
		| stat_return
		| stat_goto
		| stat_label
		| stat_label_simple
		| stat_asm
		| stat_expr
		| stat_dummy
		| ws;

statement_block = "{", {statement}, "}"

expr_atom = uint | float | char | string | identifier;
expr_parenthesis = ("(", [ws], expr, [ws], ")") | expr_atom;
expr_member_arridx_fncall = expr_parenthesis,
		{[ws],
			(".", [ws], identifier)
			| ("->", [ws], identifier)
			| ("[", [ws], expr, [ws], "]")
			| ("(", [[ws], expr, {[ws], ",", [ws], expr}], [ws], ")")
		};
expr_unary = expr_member_arridx_fncall
		| ("!", [ws], expr_unary)
		| ("+", [ws], expr_unary)
		| ("-", [ws], expr_unary)
		| ("~", [ws], expr_unary)
		| ("++", [ws], expr_unary)
		| ("--", [ws], expr_unary)
		| ("sizeof", [ws], "(", [ws], (type | expr), [ws], ")")
		| ("alignof", [ws], "(", [ws], (type | expr), [ws], ")")
		| ("cast", [ws], "<", [ws], type, [ws], ">", [ws], "(", [ws], expr, [ws], ")")
		| ("&", [ws], expr)
		| ("*", [ws], expr);
expr_mul = expr_unary,
		{[ws],
			("*", [ws], expr_unary)
			| ("/", [ws], expr_unary)
			| ("%", [ws], expr_unary)
		};
expr_add = expr_mul,
		{[ws],
			("+", [ws], expr_mul)
			| ("-", [ws], expr_mul)
		};
expr_shift = expr_add,
		{[ws],
			("<<", [ws], expr_add)
			| (">>", [ws], expr_add)
		};
expr_lt = expr_shift,
		{[ws],
			("<", [ws], expr_shift)
			| ("<=", [ws], expr_shift)
			| (">", [ws], expr_shift)
			| (">=", [ws], expr_shift)
		};
expr_eq = expr_lt,
		{[ws],
			("==", [ws], expr_lt)
			| ("!=", [ws], expr_lt)
		};
expr_band = expr_eq, {[ws], "&", [ws], expr_eq};
expr_bxor = expr_band, {[ws], "^", [ws], expr_band};
expr_bor = expr_bxor, {[ws], "|", [ws], expr_bxor};
expr_and = expr_bor, {[ws], "&&", [ws], expr_bor};
expr_or = expr_and, {[ws], "||", [ws], expr_and};
expr_assign = expr_or,
		[[ws],
			("=", [ws], expr)
			| ("+=", [ws], expr)
			| ("-=", [ws], expr)
			| ("*=", [ws], expr)
			| ("/=", [ws], expr)
			| ("%=", [ws], expr)
			| ("&=", [ws], expr)
			| ("|=", [ws], expr)
			| ("^=", [ws], expr)
			| ("<<=", [ws], expr)
			| (">>=", [ws], expr)
		];
expr = expr_assign;

module_item = stat_var
		| struct_declaration
		| struct_definition
		| function_declaration
		| function_definition
		| ws;

module = {module_item}
```

## 类型
ilang的类型系统主要由五种类型分类构成：基本类型、结构体类型、数组类型、函数类型以及指针类型。

### 基本类型
基本类型包括：char、int8、uint8、int16、uint16、int32、uint32、int64、uint64、float、double。

#### char和int8
char和int8均为8位有符号整型类型。在表达式计算以及变量定义时，ilang对char类型表达式的处理与int8类型表达式的处理相同。但是char类型一般用于表示一个表达式的值是一个8位长度的字符，而int8类型表示一个表达式的值仅仅是一个有符号8位长度的整型数值。对于变量定义而言，char和int8在内存中占据的空间大小、对占据的空间内的值的处理均相同。取值范围均为-2^7 ~ 2^7-1。

```
// 变量a与b在内存中占据的空间大小、储存的值、ilang对两者的处理均相同。
var a:char = 'A';	// 变量a的类型为char，储存着字符“A”（ASCII码为65），'A'等介于65。
var b:int8 = 65;	// 变量b的类型为int8，储存着整数65。
var c:char = 65;	
var d:int8 = 'A';
```

#### uint8
uint8为8位无符号整型类型。取值范围为0 ~ 2^8-1。

```
var a:uint8 = 123;
```

#### int16
int16为16位有符号整型类型。取值范围为-2^15 ~ 2^15-1。

```
var a:int16 = 123;
```

#### uint16
uint16为16位无符号整型类型。取值范围为0 ~ 2^16-1。

```
var a:uint16 = 123;
```

#### int32
int32为32位有符号整型类型。取值范围为-2^31 ~ 2^31-1。

```
var a:int32 = 123;
```

#### uint32
uint32为32位无符号整型类型。取值范围为0 ~ 2^32-1。

```
var a:uint32 = 123;
```

#### int64
int64为64位有符号整型类型。取值范围为-2^63 ~ 2^63-1。

```
var a:int64 = 123;
```

#### uint64
uint64为64位无符号整型类型。取值范围为0 ~ 2^64-1。

```
var a:uint64 = 123;
```

#### float
float为32位浮点数类型。取值范围为：
- 最小非正规值： ± 1.401,298,4 · 10^(-45)
- 最小正规值： ± 1.175,494,3 · 10^(-38)
- 最大值： ± 3.402,823,4 · 10^38

```
var a:float = 123.456;
```

#### double
doubl为64位浮点数类型。取值范围为：
- 最小非正规值： ± 4.940,656,458,412 · 10^(-324)
- 最小正规值： ± 2.225,073,858,507,201,4 · 10^(-308)
- 最大值： ± 1.797,693,134,862,315,7 · 10^308

```
var a:double = 123.456;
```

### 结构体类型
结构体类型是一种由一系列成员变量组成的类型，成员变量按顺序分配于内存之中。对一个结构体类型进行引用时，使用形如`struct ID`的方式，比如定义一个变量为某个结构体类型`var a:struct ID;`。结构体类型分为声明与定义，结构体声明形如`struct ID dummy`，结构体定义形如`struct ID { ...成员变量定义... }`。在定义一个结构体时，当前结构体类型对于成员变量是不可见的，即成员变量无法引用自身结构体类型。因为结构体的成员变量无法引用自身结构体类型，所以不存在自身循环引用的问题，即结构体类型A的成员变量a的类型为结构体类型A，`struct A { var a:struct A; }`是错误的。使用一个结构体类型，结构体类型必须已经被定义或已经被声明（只有声明的情况下只能通过指针引用一个结构体类型）。结构体类型不能被重复声明、不能被重复定义、不能在定义后进行声明，但是允许只有声明、只有定义以及声明后定义。

```
// 定义结构体类型A、包含3个成员变量：a、b、c，类型分别为：char、int8、uint8。
struct A {
	var a:char;
	var b:int8;
	var c:uint8;
}

// 定义结构体类型B、包含3个成员变量：a、b、c，类型分别为：struct A、int16、uint16。
struct B {
	var a:struct A;
	var b:int16;
	var c:uint16;
}

// 声明一个结构体类型D。
struct D dummy

// 定义一个变量，该变量的类型为*struct D（指向一个struct D类型的指针）。
// 在定义该变量之前只有一个关于struct D的声明，而没有具体定义，所以只能通过指针间接地引用该结构体类型。
// 如果直接定义一个类型为某结构体类型的变量，但是该结构体类型只有声明没有定义，那么该变量定义是错误的。
var pointer:*struct D;

// 声明一个结构体类型C。
struct C dummy

// 定义结构体类型C，包含3个成员变量：a、b，类型分别为：*struct C、*struct D。
struct C {
	// 定义一个成员变量a，该成员变量的类型为*struct C，因为在这之前已经声明了结构体类型C，
	// 所以该成员变量可以通过指针引用结构体类型C。在没有前置声明的情况下，结构体类型对于结构体类型内的
	// 成员变量定义是不可见的。
	var a:*struct C;

	var b:*struct D;
}

struct D {
	var a:char;
}

var xa:struct A;
var xb:struct B;
var xc:struct C;
var xd:struct D;
```

#### 结构体类型声明
结构体类型声明的存在是为了解决向前引用的问题。结构体类型声明形如`struct ID dummy`，把结构体类型定义成员的块替换为关键字dummy，就变为结构体类型声明，表示在当前并不关系结构体的成员有哪些，而只确定有这么一个结构体类型。

```
// 表示链表节点的一个结构体类型。

// 声明一个结构体类型LinkedListNode。
struct LinkedListNode dummy;

// 定义一个结构体类型LinkedListNode。
struct LinkedListNode {
	// 定义一个成员变量value，类型为*（类型于C语言的void*），用于储存数据。
	var value:*;
	
	// 定义一个成员变量next，类型为*struct LinkedListNode，用于指向下一个节点。
	// 这里可以引用结构体类型LinkedListNode是因为前面已经对结构体类型LinkedListNode进行了声明。
	var next:*struct LinkedListNode;
}
```

#### 结构体类型定义
结构体类型定义形如`struct ID { ...成员变量定义... }`，结构体类型定义后，不能再有结构体类型声明。

```
// 定义一个结构体类型Point2D。
struct Point2D {
	int32 x;
	int32 y;
}

// 定义一个结构体类型Point3D。
struct Point3D {
	int32 x;
	int32 y;
	int32 z;
}
```

### 数组类型



### 函数类型


### 指针类型


## 表达式


## 函数


## 语句


### var语句


### if语句


### while语句


### do语句


### for语句


### break语句


### continue语句


### return语句


### goto语句


### label语句


### asm语句


### 表达式语句


## 预处理指令

