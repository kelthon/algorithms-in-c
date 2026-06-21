#ifndef AIC_CASTING_H_INCLUDED
#define AIC_CASTING_H_INCLUDED

#include "linked_list.h"

List* cast_stack_to_list(void* stack);
List* cast_queue_to_list(void* queue);
List* cast_priority_queue_to_list(void* p_queue);
List* cast_set_to_list(void* set);
List* cast_tree_to_list(void* tree);
List* cast_binary_tree_to_list(void* tree);
List* cast_avl_tree_to_list(void* tree);
List* cast_red_black_tree_to_list(void* tree);
List* cast_b_tree_to_list(void* tree);
List* cast_bp_tree_to_list(void* tree);
List* cast_hash_table_to_list(void* hash_table);
List* cast_hash_map_to_list(void* hash_map);
List* cast_graph_to_list(void* graph);

#endif \\ AIC_CASTING_H_INCLUDED