#ifndef __STRUCTS_H
#define __STRUCTS_H
struct list_struct {
    sm_ref node;
    struct list_struct *next;
};

typedef enum {
    cod_field_ref,
    cod_iteration_statement,
    cod_declaration,
    cod_struct_type_decl,
    cod_compound_statement,
    cod_constant,
    cod_array_type_decl,
    cod_element_ref,
    cod_enum_type_decl,
    cod_expression_statement,
    cod_designator,
    cod_cast,
    cod_initializer,
    cod_type_specifier,
    cod_selection_statement,
    cod_comma_expression,
    cod_initializer_list,
    cod_assignment_expression,
    cod_conditional_operator,
    cod_enumerator,
    cod_reference_type_decl,
    cod_field,
    cod_label_statement,
    cod_identifier,
    cod_operator,
    cod_jump_statement,
    cod_subroutine_call,
    cod_return_statement,
    cod_last_node_type
} cod_node_type;

typedef struct {
    sm_ref sm_field_ref;
    sm_ref struct_ref;
    char* lx_field;
    srcpos lx_srcpos;
} field_ref;

typedef struct {
    sm_ref test_expr;
    sm_ref iter_expr;
    srcpos lx_srcpos;
    sm_ref statement;
    sm_ref init_expr;
    sm_ref post_test_expr;
    int cg_end_label;
    int cg_iter_label;
} iteration_statement;

typedef struct {
    srcpos lx_srcpos;
    int is_extern;
    char* id;
    int cg_oprnd;
    sm_list params;
    int static_var;
    int is_typedef;
    sm_ref init_value;
    int const_var;
    void* cg_address;
    int cg_type;
    int param_num;
    sm_ref sm_complex_type;
    int varidiac_subroutine_param_count;
    void* closure_id;
    int addr_taken;
    sm_list type_spec;
    int is_subroutine;
    sm_ref freeable_complex_type;
} declaration;

typedef struct {
    sm_list fields;
    enc_info encode_info;
    srcpos lx_srcpos;
    char* id;
    int cg_size;
} struct_type_decl;

typedef struct {
    sm_list decls;
    sm_list statements;
} compound_statement;

typedef struct {
    char* const_val;
    char* freeable_name;
    int token;
    srcpos lx_srcpos;
} constant;

typedef struct {
    int cg_element_type;
    srcpos lx_srcpos;
    sm_ref element_ref;
    sm_ref freeable_complex_element_type;
    int static_var;
    int cg_static_size;
    int cg_element_size;
    sm_ref size_expr;
    dimen_p dimensions;
    sm_ref sm_complex_element_type;
    sm_list type_spec;
    sm_ref sm_dynamic_size;
} array_type_decl;

typedef struct {
    sm_ref expression;
    sm_ref sm_containing_structure_ref;
    sm_ref sm_complex_element_type;
    int this_index_dimension;
    sm_ref array_ref;
    srcpos lx_srcpos;
    int cg_element_type;
} element_ref;

typedef struct {
    sm_list enums;
    srcpos lx_srcpos;
    char* id;
} enum_type_decl;

typedef struct {
    sm_ref expression;
} expression_statement;

typedef struct {
    sm_ref expression;
    char* id;
} designator;

typedef struct {
    sm_list type_spec;
    sm_ref expression;
    int cg_type;
    srcpos lx_srcpos;
    sm_ref sm_complex_type;
} cast;

typedef struct {
    sm_list designation;
    sm_ref initializer;
} initializer;

typedef struct {
    int token;
    srcpos lx_srcpos;
    sm_ref created_type_decl;
} type_specifier;

typedef struct {
    sm_ref then_part;
    sm_ref else_part;
    sm_ref conditional;
    srcpos lx_srcpos;
} selection_statement;

typedef struct {
    sm_ref right;
    srcpos lx_srcpos;
    sm_ref left;
} comma_expression;

typedef struct {
    sm_list initializers;
} initializer_list;

typedef struct {
    operator_t op;
    srcpos lx_srcpos;
    sm_ref right;
    sm_ref left;
    int cg_type;
} assignment_expression;

typedef struct {
    sm_ref e2;
    sm_ref e1;
    srcpos lx_srcpos;
    int result_type;
    sm_ref condition;
} conditional_operator;

typedef struct {
    sm_ref const_expression;
    char* id;
    int enum_value;
} enumerator;

typedef struct {
    srcpos lx_srcpos;
    int kernel_ref;
    int cg_referenced_type;
    int cg_referenced_size;
    sm_ref freeable_complex_referenced_type;
    sm_ref sm_complex_referenced_type;
    sm_list type_spec;
    char* name;
} reference_type_decl;

typedef struct {
    sm_ref sm_complex_type;
    char* string_type;
    int cg_size;
    sm_ref freeable_complex_type;
    char* name;
    sm_list type_spec;
    int cg_offset;
    int cg_type;
} field;

typedef struct {
    sm_ref statement;
    int cg_label;
    char* name;
} label_statement;

typedef struct {
    char* id;
    srcpos lx_srcpos;
    int cg_type;
    sm_ref sm_declaration;
} identifier;

typedef struct {
    operator_t op;
    srcpos lx_srcpos;
    int result_type;
    sm_ref right;
    sm_ref left;
    int operation_type;
} operator;

typedef struct {
    int continue_flag;
    srcpos lx_srcpos;
    sm_ref sm_target_stmt;
    char* goto_target;
} jump_statement;

typedef struct {
    sm_list arguments;
    srcpos lx_srcpos;
    sm_ref sm_func_ref;
} subroutine_call;

typedef struct {
    int cg_func_type;
    srcpos lx_srcpos;
    sm_ref expression;
} return_statement;

typedef union {
   field_ref field_ref;
   iteration_statement iteration_statement;
   declaration declaration;
   struct_type_decl struct_type_decl;
   compound_statement compound_statement;
   constant constant;
   array_type_decl array_type_decl;
   element_ref element_ref;
   enum_type_decl enum_type_decl;
   expression_statement expression_statement;
   designator designator;
   cast cast;
   initializer initializer;
   type_specifier type_specifier;
   selection_statement selection_statement;
   comma_expression comma_expression;
   initializer_list initializer_list;
   assignment_expression assignment_expression;
   conditional_operator conditional_operator;
   enumerator enumerator;
   reference_type_decl reference_type_decl;
   field field;
   label_statement label_statement;
   identifier identifier;
   operator operator;
   jump_statement jump_statement;
   subroutine_call subroutine_call;
   return_statement return_statement;
} sm_union;

struct sm_struct {
    cod_node_type node_type;
    int visited;
    sm_union node;
};
#endif
extern sm_ref cod_new_field_ref();
extern sm_ref cod_new_iteration_statement();
extern sm_ref cod_new_declaration();
extern sm_ref cod_new_struct_type_decl();
extern sm_ref cod_new_compound_statement();
extern sm_ref cod_new_constant();
extern sm_ref cod_new_array_type_decl();
extern sm_ref cod_new_element_ref();
extern sm_ref cod_new_enum_type_decl();
extern sm_ref cod_new_expression_statement();
extern sm_ref cod_new_designator();
extern sm_ref cod_new_cast();
extern sm_ref cod_new_initializer();
extern sm_ref cod_new_type_specifier();
extern sm_ref cod_new_selection_statement();
extern sm_ref cod_new_comma_expression();
extern sm_ref cod_new_initializer_list();
extern sm_ref cod_new_assignment_expression();
extern sm_ref cod_new_conditional_operator();
extern sm_ref cod_new_enumerator();
extern sm_ref cod_new_reference_type_decl();
extern sm_ref cod_new_field();
extern sm_ref cod_new_label_statement();
extern sm_ref cod_new_identifier();
extern sm_ref cod_new_operator();
extern sm_ref cod_new_jump_statement();
extern sm_ref cod_new_subroutine_call();
extern sm_ref cod_new_return_statement();
typedef void (*cod_apply_func)(sm_ref node, void *data);
typedef void (*cod_apply_list_func)(sm_list list, void *data);
extern void cod_apply(sm_ref node, cod_apply_func pre_func, cod_apply_func post_func, cod_apply_list_func list_func, void *data);
extern void cod_print(sm_ref node);
extern void cod_free(sm_ref node);
extern void cod_free_list(sm_list list, void *junk);
extern void cod_rfree(sm_ref node);
extern void cod_rfree_list(sm_list list, void *junk);
extern sm_ref cod_copy(sm_ref node);
extern sm_list cod_copy_list(sm_list list);
extern sm_list cod_append_list(sm_list list1, sm_list list2);
extern srcpos cod_get_srcpos(sm_ref expr);
