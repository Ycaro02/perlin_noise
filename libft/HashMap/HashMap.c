/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashMap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfour <nfour@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 19:35:27 by nfour             #+#    #+#             */
/*   Updated: 2024/05/06 12:12:20 by nfour            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "HashMap.h"
#include "primeNumber.h"

/* Basic function you can provide to hashmap_init */
void hashmap_entry_free(void *entry) {
	HashMap_entry *e = (HashMap_entry *)entry;
	if (e->value) {
		free(e->value); /* replace this by your free function */
	}
	free(e); /* free the entry t_list node */
}

u64 hash_block_position(s32 x, s32 y, s32 z) {
    u64 key = ((u64)x << 42) | ((u64)y << 21) | (u64)z;
    return (key);
}

HashMap *hashmap_init(size_t capacity, void (*free_obj)(void *obj)) {
	HashMap *map = NULL;

	if (!free_obj) {
		ft_printf_fd(2, "hashmap_init: free_obj is NULL user need to provide free function\n");
		return (NULL);
	}
	
	map = ft_calloc(sizeof(HashMap), 1);
	if (!map) {
		return (NULL);
	}

	size_t prime_capacity = GET_NEXT_PRIME(capacity);

	map->entries = ft_calloc(sizeof(HashMap_entry), prime_capacity);
	if (!map->entries) {
		free(map);
		return (NULL);
	}

	map->capacity = prime_capacity;
	map->size = 0;
	map->free_obj = free_obj;
	return (map);
}

void hashmap_destroy(HashMap *map) {
	if (!map) {
		return ;
	}
    for (size_t i = 0; i < map->capacity; i++) {
        ft_lstclear(&map->entries[i], map->free_obj);
    }
	free(map->entries); /* free entry t_list ** array */
	free(map);			/* free map */
}

void *hashmap_get(HashMap *map, BlockPos p) {
	u64		key = hash_block_position(p.x, p.y, p.z);
	size_t	index = HASHMAP_INDEX(key, map->capacity);

	t_list *entry = map->entries[index];
	while (entry) {
		HashMap_entry *e = (HashMap_entry *)entry->content;
		if (HASHMAP_SAME_ENTRY(e, key, p.x, p.y, p.z)) {
			return (e->value);
		}
		entry = entry->next;
	}
	return (NULL);
}

u8 hashmap_set_entry(HashMap *map, BlockPos p, void *value) {
	u64		key = hash_block_position(p.x, p.y, p.z);
	size_t	index = HASHMAP_INDEX(key, map->capacity);

	/* Check if the entry already exist */
	t_list *current = map->entries[index];
	while (current) {
		HashMap_entry *e = (HashMap_entry *)current->content;
		if (HASHMAP_SAME_ENTRY(e, key, p.x, p.y, p.z)) {
			if (e->value) {
				free(e->value);
			}
			e->value = value;
			return (HASHMAP_UPT_ENTRY);
		}
		current = current->next;
	}

	t_list *entry = ft_lstnew(ft_calloc(sizeof(HashMap_entry), 1));
	if (!entry) {
		return (HASHMAP_MALLOC_ERROR);
	}
	HashMap_entry *e = (HashMap_entry *)entry->content;
	e->origin_data.x = p.x;
	e->origin_data.y = p.y;
	e->origin_data.z = p.z;
	e->key = key;
	e->value = value;
	ft_lstadd_back(&map->entries[index], entry);
	(map->size)++;
	return (HASHMAP_ADD_ENTRY);
}


s8 hashmap_expand(HashMap *map) 
{
	/* Compute new size */
    size_t new_capacity = (map->capacity * 2);
	new_capacity = GET_NEXT_PRIME(new_capacity);

	/* Allocate new entries array */
    t_list **new_entries = ft_calloc(sizeof(t_list *), new_capacity);
    if (!new_entries) {
        return (FALSE);
    }

    /* Rehash and move existing entries to the new array */
    for (size_t i = 0; i < map->capacity; i++) {
        t_list *current = map->entries[i];
        while (current) {
            HashMap_entry *entry = (HashMap_entry *)current->content;
            size_t new_index = HASHMAP_INDEX(entry->key, new_capacity); /* Calculate new index */
            t_list *new_entry = ft_lstnew(entry);
            if (!new_entry) {
                /* Handle memory allocation failure Free memory and return (FALSE) */
                ft_lstclear(&new_entries[i], free);
                free(new_entries);
                return (FALSE);
            }
            ft_lstadd_back(&new_entries[new_index], new_entry);
            current = current->next;
        }
    }

    for (size_t i = 0; i < map->capacity; i++) {
		ft_lstclear_nodeptr(map->entries + i);
	}
    /* Free old entries */
    free(map->entries);
    
    /* Update hashmap with new capacity and entries */
    map->entries = new_entries;
    map->capacity = new_capacity;

    return (TRUE); /* Expansion successful */
}

size_t hashmap_size(HashMap *map) {
    return (map->size);
}

size_t hashmap_capacity(HashMap *map) {
    return (map->capacity);
}

HashMap_it hashmap_iterator(HashMap *map) {
    HashMap_it it;

    it._map = map;
    it._idx = 0;
	it._current = NULL;
    return (it);
}

s8 hashmap_next(HashMap_it *it) {
    HashMap *map = it->_map;
	t_list *entry = NULL;

    /* Loop through the entries array */
    while (it->_idx < map->capacity) {
        entry = map->entries[it->_idx];
        if (entry != NULL) { /* Found a non-empty list */
            if (it->_current == NULL) { /*  If it's the first node in the list, set it as the current node */
                it->_current = entry;
            } else { /* Otherwise, move to the next node in the list */
                it->_current = it->_current->next;
            }
            if (it->_current != NULL) {
                /* Go to the next entry list */
                HashMap_entry *hm_entry = it->_current->content;
                it->key = hm_entry->key;
                it->value = hm_entry->value;
                return (TRUE);
            }
        }
        (it->_idx)++;
        it->_current = NULL; /* Reset the list node pointer for the next iteration */
    }
    /* No more non-empty entries found */
    return (FALSE);
}