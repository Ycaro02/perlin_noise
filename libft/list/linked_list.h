#ifndef DEFINE_LINKED_LIST_H
# define DEFINE_LINKED_LIST_H

# include "../libft.h"
# include "../basic_define.h"

 /********************************************************************
 *                        DATA STRUCTURES                           *
 ********************************************************************/

/**
 * Simple linked list
 * void content need to be casted to acces value
*/
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

 /********************************************************************
 *                        PUBLIC FUNCTIONS                          *
 ********************************************************************/

/**
 *	@brief Create a new linked list node
 *	@param content content of the node (must be allocated)
 *	@return new allocated node
*/
t_list			*ft_lstnew(void *content);

/**
 * @brief Add a new node at the beginning of the list
 * @param lst pointer to the first node of the list
 * @param new new node to add
*/
void			ft_lstadd_front(t_list **lst, t_list *new);

/**
 * @brief Add a new node at the end of the list
 * @param lst pointer to the first node of the list
 * @param new new node to add
*/
void			ft_lstadd_back(t_list **lst, t_list *new);

/**
 * @brief Return the number of elements in a list
*/
int				ft_lstsize(t_list *lst);

/**
 * @brief Return the last node of a list
*/
t_list			*ft_lstlast(t_list *lst);

/**
 * @brief Delete the first node of the list
 * @param lst pointer to the first node of the list
 * @param del function to delete the content of the node
*/
void			ft_lstdelone(t_list *lst, void (*del)(void *));

/**
 * @brief Delete all nodes of the list
 * @param lst pointer to the first node of the list
 * @param del function to delete the content of the node
*/
void			ft_lstclear(t_list **lst, void (*del)(void*));

/**
 * @brief Apply a function to each node of the list
 * @param lst pointer to the first node of the list
 * @param f function to apply
*/
void			ft_lstiter(t_list *lst, void (*f)(void *));

/**
 * @brief Apply a function to each node of the list and create a new list
 * @param lst pointer to the first node of the list
 * @param f function to apply
 * @param del function to delete the content of the node
 * @return new list
*/
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/**
 * @brief Join two list
 * @param first first list
 * @param second second list
 * @return new list
*/
t_list 			*ft_lstjoin(t_list *first, t_list *second);

/**
 * @brief Free only list node not his content
*/
void 			ft_lstclear_nodeptr(t_list **lst);

/**
 * @brief Remove a node from the list
 * @param lst pointer to the first node of the list
 * @param ref reference to the node to remove
 * @param del function to delete the content of the node
 * @param condition function to check if the node need to be removed
*/
void			ft_lst_remove_if(t_list **list, void *ref, void (*del)(), int (*condition)());

/**
 * @brief Get the content of the node at the index
 * @param lst pointer to the first node of the list
 * @param index index of the node
 * @return content of the node
*/
void 			*get_lst_index_content(t_list *lst, int index);

/**
 * @brief List clear
*/
void			lst_clear(t_list **lst, void (*del)(void*));

/**
 * @brief Reverse the list
*/
void 			reverse_lst(t_list **lst);

/**
 * @brief Sort the list
 * @param lst pointer to the first node of the list
 * @param cmp function to compare two nodes
*/
void			list_sort(t_list **lst, int (*cmp)());

/**
 * @brief Convert a list to an allocated array
 * @param lst pointer to the first node of the list
 * @param lst_size number of elements in the list
 * @param type_size size of the type of the content
 * @return new allocated array
*/
void			*list_to_array(t_list *lst, u32 lst_size, u32 type_size);
 # endif /* DEFINE_LINKED_LIST_H */