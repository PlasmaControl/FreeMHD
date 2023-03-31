#ifndef __STRUCTS_H
#define __STRUCTS_H
struct list_struct {
    sm_ref node;
    struct list_struct *next;
};

typedef enum {
    cod_enum_type_decl,
    cod_element_ref,
    cod_enumerator,
    cod_iteration_statement,
    cod_conditional_operator,
    cod_expression_statement,
    cod_constant,
    cod_compound_statement,
    cod_field,
    cod_array_type_decl,
    cod_initializer_list,
    cod_initializer,
    cod_jump_statement,
    cod_label_statement,
    cod_subroutine_call,
    cod_operator,
    cod_declaration,
    cod_designator,
    cod_selection_statement,
    cod_type_specifier,
    cod_reference_type_decl,
    cod_field_ref,
    cod_comma_expression,
    cod_struct_type_decl,
    cod_return_statement,
    cod_assignment_expression,
    cod_cast,
    cod_identifier,
    cod_last_node_type
} cod_node_type;

typedef struct {
    srcpos lx_srcpos;
    sm_list enums;
    char* id;
} enum_type_decl;

typedef struct {
    sm_ref sm_containing_structure_ref;
    int cg_element_type;
    int this_index_dimension;
    sm_ref sm_complex_element_type;
    sm_ref expression;
    sm_ref array_ref;
    srcpos lx_srcpos;
} element_ref;

typedef struct {
    sm_ref const_expression;
    char* id;
    int enum_value;
} enumerator;

typedef struct {
    sm_ref statement;
    int cg_iter_label;
    srcpos lx_srcpos;
    int cg_end_label;
    sm_ref test_expr;
    sm_ref post_test_expr;
    sm_ref init_expr;
    sm_ref iter_expr;
} iteration_statement;

typedef struct {
    int result_type;
    srcpos lx_srcpos;
    sm_ref condition;
    sm_ref e2;
    sm_ref e1;
} conditional_operator;

typedef struct {
    sm_ref expression;
} expression_statement;

typedef struct {
    char* const_val;
    srcpos lx_srcpos;
    int token;
    char* freeable_name;
} constant;

typedef struct {
    sm_list decls;
    sm_list statements;
} compound_statement;

typedef struct {
    char* string_type;
    sm_list type_spec;
    char* name;
    sm_ref freeable_complex_type;
    int cg_type;
    sm_ref sm_complex_type;
    int cg_size;
    int cg_offset;
} field;

typedef struct {
    int cg_static_size;
    srcpos lx_srcpos;
    sm_ref sm_dynamic_size;
    sm_list type_spec;
    int cg_element_type;
    int cg_element_size;
    sm_ref size_expr;
    sm_ref sm_complex_element_type;
    sm_ref element_ref;
    sm_ref freeable_complex_element_type;
    int static_var;
    dimen_p dimensions;
} array_type_decl;

typedef struct {
    sm_list initializers;
} initializer_list;

typedef struct {
    sm_ref initializer;
    sm_list designation;
} initializer;

typedef struct {
    srcpos lx_srcpos;
    int continue_flag;
    sm_ref sm_target_stmt;
    char* goto_target;
} jump_statement;

typedef struct {
    char* name;
    int cg_label;
    sm_ref statement;
} label_statement;

typedef struct {
    srcpos lx_srcpos;
    sm_list arguments;
    sm_ref sm_func_ref;
} subroutine_call;

typedef struct {
    int result_type;
    sm_ref right;
    sm_ref left;
    int operation_type;
    operator_t op;
    srcpos lx_srcpos;
} operator;

typedef struct {
    int is_subroutine;
    void* cg_address;
    int const_var;
    sm_ref freeable_complex_type;
    sm_ref sm_complex_type;
    int is_extern;
    int param_num;
    char* id;
    int cg_oprnd;
    int varidiac_subroutine_param_count;
    srcpos lx_srcpos;
    int cg_type;
    sm_list type_spec;
    int is_typedef;
    sm_ref init_value;
    sm_list params;
    int addr_taken;
    void* closure_id;
    int static_var;
} declaration;

typedef struct {
    sm_ref expression;
    char* id;
} designator;

typedef struct {
    sm_ref else_part;
    sm_ref conditional;
    srcpos lx_srcpos;
    sm_ref then_part;
} selection_statement;

typedef struct {
    srcpos lx_srcpos;
    sm_ref created_type_decl;
    int token;
} type_specifier;

typedef struct {
    int cg_referenced_type;
    srcpos lx_srcpos;
    sm_list type_spec;
    char* name;
    int kernel_ref;
    sm_ref freeable_complex_referenced_type;
    int cg_referenced_size;
    sm_ref sm_complex_referenced_type;
} reference_type_decl;

typedef struct {
    char* lx_field;
    srcpos lx_srcpos;
    sm_ref sm_field_ref;
    sm_ref struct_ref;
} field_ref;

typedef struct {
    srcpos lx_srcpos;
    sm_ref left;
    sm_ref right;
} comma_expression;

typedef struct {
    srcpos lx_srcpos;
    sm_list fields;
    enc_info encode_info;
    char* id;
    int cg_size;
} struct_type_decl;

typedef struct {
    int cg_func_type;
    sm_ref expression;
    srcpos lx_srcpos;
} return_statement;

typedef struct {
    srcpos lx_srcpos;
    operator_t op;
    int cg_type;
    sm_ref left;
    sm_ref right;
} assignment_expression;

typedef struct {
    int cg_type;
    sm_list type_spec;
    sm_ref sm_complex_type;
    sm_ref expression;
    srcpos lx_srcpos;
} cast;

typedef struct {
    char* id;
    int cg_type;
    srcpos lx_srcpos;
    sm_ref sm_declaration;
} identifier;

typedef union {
   enum_type_decl enum_type_decl;
   element_ref element_ref;
   enumerator enumerator;
   iteration_statement iteration_statement;
   conditional_operator conditional_operator;
   expression_statement expression_statement;
   constant constant;
   compound_statement compound_statement;
   field field;
   array_type_decl array_type_decl;
   initializer_list initializer_list;
   initializer initializer;
   jump_statement jump_statement;
   label_statement label_statement;
   subroutine_call subroutine_call;
   operator operator;
   declaration declaration;
   designator designator;
   selection_statement selection_statement;
   type_specifier type_specifier;
   reference_type_decl reference_type_decl;
   field_ref field_ref;
   comma_expression comma_expression;
   struct_type_decl struct_type_decl;
   return_statement return_statement;
   assignment_expression assignment_expression;
   cast cast;
   identifier identifier;
} sm_union;

struct sm_struct {
    cod_node_type node_type;
    int visited;
    sm_union node;
};
#endif
extern sm_ref cod_new_enum_type_decl();
extern sm_ref cod_new_element_ref();
extern sm_ref cod_new_enumerator();
extern sm_ref cod_new_iteration_statement();
extern sm_ref cod_new_conditional_operator();
extern sm_ref cod_new_expression_statement();
extern sm_ref cod_new_constant();
extern sm_ref cod_new_compound_statement();
extern sm_ref cod_new_field();
extern sm_ref cod_new_array_type_decl();
extern sm_ref cod_new_initializer_list();
extern sm_ref cod_new_initializer();
extern sm_ref cod_new_jump_statement();
extern sm_ref cod_new_label_statement();
extern sm_ref cod_new_subroutine_call();
extern sm_ref cod_new_operator();
extern sm_ref cod_new_declaration();
extern sm_ref cod_new_designator();
extern sm_ref cod_new_selection_statement();
extern sm_ref cod_new_type_specifier();
extern sm_ref cod_new_reference_type_decl();
extern sm_ref cod_new_field_ref();
extern sm_ref cod_new_comma_expression();
extern sm_ref cod_new_struct_type_decl();
extern sm_ref cod_new_return_statement();
extern sm_ref cod_new_assignment_expression();
extern sm_ref cod_new_cast();
extern sm_ref cod_new_identifier();
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
