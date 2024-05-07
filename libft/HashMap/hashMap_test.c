#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "HashMap.h"
#include "primeNumber.h"
#include "../libft.h"

// Function to compare block positions
int compare_BlockPos(BlockPos pos1, BlockPos pos2) {
    return (pos1.x == pos2.x && pos1.y == pos2.y && pos1.z == pos2.z);
}

// Function to compare values
int compare_values(void *value1, void *value2) {
    // For this example, assuming values are strings
    return ft_strcmp((char *)value1, (char *)value2) == 0;
}

// Test function for the hashmap
void hashmap_test() {
    // Initialize the hashmap
    HashMap *map = hashmap_init(8, hashmap_entry_free);
    assert(map != NULL);

    assert(hashmap_capacity(map) == find_next_prime(8));

    // Add entries
    BlockPos pos1 = {1, 2, 3};
    BlockPos pos2 = {4, 5, 6};
    BlockPos pos3 = {7, 8, 9};
    assert(hashmap_set_entry(map, pos1, ft_strdup("Value1")) == HASHMAP_ADD_ENTRY);
    assert(hashmap_set_entry(map, pos2, ft_strdup("Value2")) == HASHMAP_ADD_ENTRY);
    assert(hashmap_set_entry(map, pos3, ft_strdup("Value3")) == HASHMAP_ADD_ENTRY);

    // Retrieve values from the hashmap and verify
    assert(compare_values(hashmap_get(map, pos1), "Value1"));
    assert(compare_values(hashmap_get(map, pos2), "Value2"));
    assert(compare_values(hashmap_get(map, pos3), "Value3"));
    assert(hashmap_get(map, (BlockPos){10, 11, 12}) == NULL);

    // Add more entries after expansion
    assert(hashmap_set_entry(map, (BlockPos){10, 11, 12}, ft_strdup("Value4")) == HASHMAP_ADD_ENTRY);
    assert(hashmap_set_entry(map, (BlockPos){13, 14, 15}, ft_strdup("Value5")) == HASHMAP_ADD_ENTRY);

    // Retrieve values from the hashmap after expansion and verify
    assert(compare_values(hashmap_get(map, (BlockPos){10, 11, 12}), "Value4"));
    assert(compare_values(hashmap_get(map, (BlockPos){13, 14, 15}), "Value5"));

    // Update an entry
    assert(hashmap_set_entry(map, pos1, ft_strdup("UpdatedValue")) == HASHMAP_UPT_ENTRY);
    assert(compare_values(hashmap_get(map, pos1), "UpdatedValue"));

    // Remove an entry
    assert(hashmap_set_entry(map, pos2, NULL) == HASHMAP_UPT_ENTRY);
    assert(hashmap_get(map, pos2) == NULL);

    // Destroy the hashmap
    hashmap_destroy(map);
}

void hashmap_colision_test() {
    // Initialize the hashmap with a small capacity to force collisions
    HashMap *map = hashmap_init(4, hashmap_entry_free);
    assert(map != NULL);

    // Add entries with keys that collide
    assert(hashmap_set_entry(map, (BlockPos){1, 2, 3}, ft_strdup("Value1")) == HASHMAP_ADD_ENTRY);
    assert(hashmap_set_entry(map, (BlockPos){5, 6, 7}, ft_strdup("Value2")) == HASHMAP_ADD_ENTRY);
    assert(hashmap_set_entry(map, (BlockPos){9, 10, 11}, ft_strdup("Value3")) == HASHMAP_ADD_ENTRY);

    // Retrieve values from the hashmap and verify
    assert(compare_values(hashmap_get(map, (BlockPos){1, 2, 3}), "Value1"));
    assert(compare_values(hashmap_get(map, (BlockPos){5, 6, 7}), "Value2"));
    assert(compare_values(hashmap_get(map, (BlockPos){9, 10, 11}), "Value3"));

    // Add more entries with keys that collide with existing entries
    assert(hashmap_set_entry(map, (BlockPos){13, 14, 15}, ft_strdup("Value4")) == HASHMAP_ADD_ENTRY);
    assert(hashmap_set_entry(map, (BlockPos){17, 18, 19}, ft_strdup("Value5")) == HASHMAP_ADD_ENTRY);
    assert(hashmap_set_entry(map, (BlockPos){100, 108, 109}, ft_strdup("Value6")) == HASHMAP_ADD_ENTRY);
    assert(hashmap_set_entry(map, (BlockPos){117, 128, 129}, ft_strdup("Value7")) == HASHMAP_ADD_ENTRY);

    // Retrieve ALL values from the hashmap after adding more entries and verify
	assert(compare_values(hashmap_get(map, (BlockPos){1, 2, 3}), "Value1"));
    assert(compare_values(hashmap_get(map, (BlockPos){5, 6, 7}), "Value2"));
    assert(compare_values(hashmap_get(map, (BlockPos){9, 10, 11}), "Value3"));
    assert(compare_values(hashmap_get(map, (BlockPos){13, 14, 15}), "Value4"));
	assert(compare_values(hashmap_get(map, (BlockPos){1, 2, 3}), "Value1")); /* try this to time */
    assert(compare_values(hashmap_get(map, (BlockPos){17, 18, 19}), "Value5"));
	assert(compare_values(hashmap_get(map, (BlockPos){100, 108, 109}), "Value6"));
	assert(compare_values(hashmap_get(map, (BlockPos){117, 128, 129}), "Value7"));

    // Destroy the hashmap
    hashmap_destroy(map);
}

void hashmap_expand_test() {
    // Initialize the hashmap
    HashMap *map = hashmap_init(3, hashmap_entry_free); // Small initial capacity
    assert(map != NULL);

    // Add entries
    BlockPos pos1 = {1, 2, 3};
    BlockPos pos2 = {5, 6, 7}; // Will collide with pos1
    BlockPos pos3 = {9, 10, 11}; // Will collide with pos1 and pos2
    assert(hashmap_set_entry(map, pos1, ft_strdup("Value1")) == HASHMAP_ADD_ENTRY);
    assert(hashmap_set_entry(map, pos2, ft_strdup("Value2")) == HASHMAP_ADD_ENTRY);
    assert(hashmap_set_entry(map, pos3, ft_strdup("Value3")) == HASHMAP_ADD_ENTRY);

    // Retrieve values from the hashmap and verify
    assert(compare_values(hashmap_get(map, pos1), "Value1"));
    assert(compare_values(hashmap_get(map, pos2), "Value2"));
    assert(compare_values(hashmap_get(map, pos3), "Value3"));

	assert(map->capacity == 3);
    // Expand the hashmap
    assert(hashmap_expand(map));
	
	assert(map->capacity == find_next_prime(3 * 2));

    // Add more entries after expansion
    BlockPos pos4 = {13, 14, 15}; // No collision with existing entries after expansion
    BlockPos pos5 = {17, 18, 19}; // No collision with existing entries after expansion
    assert(hashmap_set_entry(map, pos4, ft_strdup("Value4")) == HASHMAP_ADD_ENTRY);
    assert(hashmap_set_entry(map, pos5, ft_strdup("Value5")) == HASHMAP_ADD_ENTRY);

    // Retrieve values from the hashmap after adding more entries and verify
    assert(compare_values(hashmap_get(map, pos4), "Value4"));
    assert(compare_values(hashmap_get(map, pos5), "Value5"));
    assert(compare_values(hashmap_get(map, pos1), "Value1"));
    assert(compare_values(hashmap_get(map, pos2), "Value2"));
    assert(compare_values(hashmap_get(map, pos3), "Value3"));
    assert(compare_values(hashmap_get(map, pos5), "Value5"));


    // Destroy the hashmap
    hashmap_destroy(map);
}


// Test function for hashmap_size
void hashmap_size_test() {
    HashMap *map = hashmap_init(11, hashmap_entry_free);
    assert(map != NULL);

    assert(hashmap_size(map) == 0); // Initially, the length should be 0

    hashmap_set_entry(map, (BlockPos){1, 2, 3}, ft_strdup("Value1"));
    assert(hashmap_size(map) == 1); // After adding one entry, the length should be 1

    hashmap_set_entry(map, (BlockPos){4, 5, 6}, ft_strdup("Value2"));
    hashmap_set_entry(map, (BlockPos){7, 8, 9}, ft_strdup("Value3"));
    assert(hashmap_size(map) == 3); // After adding two more entries, the length should be 3

    hashmap_destroy(map);
}

// Test function for hashmap_iterator and hashmap_next
void hashmap_iterator_test() {
    HashMap *map = hashmap_init(8, hashmap_entry_free);
    assert(map != NULL);

    hashmap_set_entry(map, (BlockPos){1, 2, 3}, ft_strdup("Value1"));
    hashmap_set_entry(map, (BlockPos){4, 5, 6}, ft_strdup("Value2"));
    hashmap_set_entry(map, (BlockPos){7, 8, 9}, ft_strdup("Value3"));

    // Initialize the iterator
    HashMap_it it = hashmap_iterator(map);
    assert(it._map == map);
    assert(it._idx == 0);
    assert(it._current == NULL);

    // Test iterating through entries
    assert(hashmap_next(&it)); // Move to the first entry
    assert(it.key == hash_block_position(1, 2, 3));
    assert(strcmp((char *)it.value, "Value1") == 0);

    assert(hashmap_next(&it)); // Move to the first entry
    assert(it.key == hash_block_position(4, 5, 6));
    assert(strcmp((char *)it.value, "Value2") == 0);

    assert(hashmap_next(&it)); // Move to the third entry
	assert(it.key == hash_block_position(7, 8, 9));
    assert(strcmp((char *)it.value, "Value3") == 0);

    assert(!hashmap_next(&it)); // No more entries

    hashmap_destroy(map);
}

int main() {
    hashmap_test();
    printf(GREEN"hashmap_test passed successfully!\n"RESET);
	hashmap_colision_test();
    printf(GREEN"hashmap_colision_test successfully!\n"RESET);
	hashmap_expand_test();
	printf(GREEN"hashmap_expand_test successfully!\n"RESET);
	hashmap_size_test();
	printf(GREEN"hashmap_size_test passed successfully!\n"RESET);
	hashmap_iterator_test();
	printf(GREEN"hashmap_iterator_test passed successfully!\n"RESET);
    return 0;
}
