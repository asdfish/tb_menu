#include <tb_menu.h>
#include <stdlib.h>
#include <string.h>

// private functions
inline unsigned int max(unsigned int x, unsigned int y) {
  return x > y ? x : y;
}

inline unsigned int min(unsigned int x, unsigned int y) {
  return x < y ? x : y;
}

inline void tb_menu_free_items(struct TbMenu* menu) {
  if(menu->items != NULL)
    free(menu->items);
  menu->items = NULL;
  menu->items_length = 0;
}

inline void tb_menu_free_filtered_items(struct TbMenu* menu) {
  if(menu->filtered_items != NULL)
    free(menu->filtered_items);
  menu->filtered_items = NULL;
  menu->filtered_items_length = 0;
}

// public functions

int tb_menu_draw(struct TbMenu* menu) {
  if(menu == NULL)
    return TBM_FAILURE_NULL_ARGS;

  if(menu->cursor < menu->camera)
    menu->camera = menu->cursor;
  if(menu->cursor > menu->camera + menu->height - 1)
    menu->camera = menu->cursor - menu->height + 1;

  struct TbMenuItem** items = NULL;
  unsigned int items_length = 0;

  if(tb_menu_get_items(menu, &items, &items_length) != TBM_SUCCESS)
    return TBM_FAILURE_NULL_ARGS;

  tb_printf(69, 0, 0, 0, "%u", menu->items_length);
  tb_printf(69, 1, 0, 0, "%u", items_length);

  for(unsigned int i = 0; i < menu->height; i ++) {
    unsigned int item_y = menu->camera + i;
    unsigned int cell_y = menu->y + i;

    if(item_y >= items_length) {
      for(unsigned int j = 0; j < menu->width; j ++)
        tb_set_cell(menu->x + j, cell_y, ' ', 0, menu->background);
      continue;
    }

    struct TbMenuItem* item = *(items + item_y);

    bool reverse_color = false;
    if(menu->enable_reversed_colors &&
        (item_y == menu->cursor ||
        (menu->enable_selection &&
         item_y >= min(menu->selection, menu->cursor) &&
         item_y <= max(menu->selection, menu->cursor))))
      reverse_color = true;

    uintattr_t foreground = reverse_color ? item->foreground_reversed : item->foreground;
    uintattr_t background = reverse_color ? menu->background_reversed : menu->background;

    for(unsigned int j = 0; j < menu->width; j ++) {
      char next_char = j < strlen(item->contents) ? item->contents[j] : ' ';
      tb_set_cell(menu->x + j, cell_y, next_char, foreground, background);
    }
  }

  return TBM_SUCCESS;
}

int tb_menu_filter(struct TbMenu* menu, const char* filter) {
  if(menu == NULL)
    return TBM_FAILURE_NULL_ARGS;

  tb_menu_free_filtered_items(menu);

  if(filter == NULL || strlen(filter) == 0)
    return TBM_SUCCESS;

  unsigned int filtered_items_length = 0;
  for(unsigned int i = 0; i < menu->items_length; i ++)
    if(strstr((*(menu->items + i))->contents, filter) != NULL)
      filtered_items_length ++;

  if(filtered_items_length == 0)
    return TBM_SUCCESS;

  if((menu->filtered_items = (struct TbMenuItem**) malloc(filtered_items_length * sizeof(struct TbMenuItem*))) == NULL)
    return TBM_FAILURE_MALLOC;

  for(unsigned int i = 0, j = 0; i < menu->items_length; i ++)
    if(strstr((*(menu->items + i))->contents, filter) != NULL) {
      if(j > filtered_items_length)
        break;

      *(menu->filtered_items + j) = *(menu->items + i);
      j ++;
    }
  menu->filtered_items_length = filtered_items_length;

  return TBM_SUCCESS;
}

int tb_menu_get_items(struct TbMenu* menu, struct TbMenuItem*** output, unsigned int* output_length) {
  if(menu == NULL)
    return TBM_FAILURE_NULL_ARGS;

  if(output == NULL && output_length == NULL)
    return TBM_SUCCESS;

  if(output != NULL)
    *output = (menu->filtered_items != NULL ? menu->filtered_items : menu->items);
  if(output_length != NULL)
    *output_length = (menu->filtered_items != NULL ? menu->filtered_items_length : menu->items_length);

  return TBM_SUCCESS;
}

int tb_menu_init(struct TbMenu* menu) {
  if(menu == NULL)
    return TBM_FAILURE_NULL_ARGS;

  menu->camera = 0;
  menu->cursor = 0;

  menu->enable_selection = false;
  menu->selection = 0;

  menu->items = NULL;
  menu->items_length = 0;

  menu->filtered_items = NULL;
  menu->filtered_items_length = 0;
  return TBM_SUCCESS;
}

int tb_menu_move_cursor(struct TbMenu* menu, int step) {
  if(menu == NULL)
    return TBM_FAILURE_NULL_ARGS;

  if((int) menu->cursor + step > 0)
    menu->cursor += step;
  else
    menu->cursor = 0;

  unsigned int items_length = 0;
  if(tb_menu_get_items(menu, NULL, &items_length) != TBM_SUCCESS)
    return TBM_FAILURE_NULL_ARGS;
  if(menu->cursor > items_length - 1)
    menu->cursor = items_length - 1;

  return TBM_SUCCESS;
}

int tb_menu_set_items(struct TbMenu* menu, struct TbMenuItem* items, unsigned int items_length) {
  if(menu == NULL)
    return TBM_FAILURE_NULL_ARGS;

  tb_menu_free_items(menu);
  if(items == NULL || items_length == 0)
    return EXIT_SUCCESS;

  menu->items = (struct TbMenuItem**) malloc(items_length * sizeof(struct TbMenuItem*));
  if(menu->items == NULL)
    return TBM_FAILURE_MALLOC;

  for(unsigned int i = 0; i < items_length; i ++)
    *(menu->items + i) = items + i;
  menu->items_length = items_length;

  return TBM_SUCCESS;
}

int tb_menu_uninit(struct TbMenu* menu) {
  if(menu == NULL)
    return TBM_FAILURE_NULL_ARGS;

  tb_menu_free_items(menu);
  tb_menu_free_filtered_items(menu);

  return TBM_SUCCESS;
}
