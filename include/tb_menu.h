#ifndef TB_MENU_H
#define TB_MENU_H

#ifndef TBM_SUCCESS
#define TBM_SUCCESS 0
#endif
#ifndef TBM_FAILURE_NULL_ARGS
#define TBM_FAILURE_NULL_ARGS -1
#endif

#include <termbox2.h>
#include <stdbool.h>

struct TbMenuItem {
  uintattr_t foreground;
  uintattr_t foreground_reversed;
  const char* contents;
};
struct TbMenu {
  unsigned int x, y, width, height;
  unsigned int camera, cursor;

  uintattr_t background, background_reversed;

  bool selected;                              // do not change
  unsigned int selection;                     // do not change

  struct MenuItem** items;                    // do not change
  unsigned int items_length;                  // do not change

  struct MenuItem** filtered_items;           // do not change
  unsigned int filtered_items_length;         // do not change
};

extern int tb_menu_draw(struct TbMenu* menu);
extern int tb_menu_init(struct TbMenu* menu);
extern int tb_menu_uninit(struct TbMenu* menu);

#endif
