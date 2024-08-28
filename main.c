#include "luigi2 (beta).h"
#include <assert.h>
#include "str.h"
#include "sudoku.h"

typedef struct {
    UITextbox* elements[9][9];
} Textboxes;

typedef struct {
    Textboxes textboxes;
    UILabel* label;
} Context;

int TextboxMessage(UIElement* element, UIMessage msg, int di, void* dp) {
    (void)element;
    (void)di;
    (void)dp;
    if (msg == UI_MSG_GET_WIDTH) {
        return 20;
    }
    return 0;
}

int ButtonMessage(UIElement *element, UIMessage message, int di, void *dp) {
    (void)element;
    (void)di;
    (void)dp;
	if (message == UI_MSG_CLICKED) {
        UIButton* button = (UIButton*)element;
        Context* ctx = (Context*)button->e.cp;
        Textboxes* textboxes = &ctx->textboxes;
        UILabel* label = ctx->label;
        u8 grid[81] = {0};
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                assert(textboxes->elements[row][col]->bytes >= 0);
                assert(textboxes);
                if(textboxes->elements[row][col]->string) {
                    sv textbox_sv = sv_from_parts(textboxes->elements[row][col]->string, (size_t)textboxes->elements[row][col]->bytes);
                    sv_trim_in_place(&textbox_sv);
                    ParseIntResult result = sv_parse_int(textbox_sv);
                    bool valid = result.err == PARSE_INT_ERR_EMPTY_STRING || result.err == PARSE_INT_OK;
                    bool in_range = 0 <= result.val && result.val <= 9;
                    if (valid && in_range) {
                        grid[row+9*col] = (u8)result.val;
                    }
                    else {
                        UILabelSetContent(label, "Invalid input", -1);
                        UIElementRefresh(&label->e);
                        UIElementRefresh(label->e.parent);
                        return 0;
                    }
                }
            }
        }
        if (solve(grid)) {
            for (int row = 0; row < 9; row++) {
                for (int col = 0; col < 9; col++) {
                    assert(textboxes->elements[row][col]->bytes >= 0);
                    char chr = grid[row+9*col] + '0';
                    UITextboxClear(textboxes->elements[row][col], 1);
                    UITextboxReplace(textboxes->elements[row][col], &chr, 1, 1);
                }
            }
            UILabelSetContent(label, "Solved", -1);
            UIElementRefresh(&label->e);
            UIElementRefresh(label->e.parent);
        }
        else {
            UILabelSetContent(label, "Unsolvable", -1);
            UIElementRefresh(&label->e);
            UIElementRefresh(label->e.parent);
        }
	}
	return 0;
}

int ClearButtonMessage(UIElement *element, UIMessage message, int di, void *dp) {
    (void)element;
    (void)di;
    (void)dp;
    if (message == UI_MSG_CLICKED) {
        UIButton* button = (UIButton*)element;
        Context* ctx = (Context*)button->e.cp;
        Textboxes* textboxes = &ctx->textboxes;
        UILabel* label = ctx->label;
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                assert(textboxes->elements[row][col]->bytes >= 0);
                UITextboxClear(textboxes->elements[row][col], 1);
            }
        }
        UILabelSetContent(label, "", -1);
        UIElementRefresh(&label->e);
        UIElementRefresh(label->e.parent);
    }
    return 0;
}

int main(void) {
    UIInitialise();
    UIWindow* window = UIWindowCreate(0, 0, "Sudoku Solver", 800, 600);
    UIPanel* panel = UIPanelCreate(&window->e, UI_PANEL_COLOR_1 | UI_PANEL_MEDIUM_SPACING);
    UIPanel* gridpanel = UIPanelCreate(&panel->e, UI_PANEL_COLOR_1 | UI_PANEL_MEDIUM_SPACING);
    Textboxes textboxes = {0};
    for (int row = 0; row < 9; row++) {
        if (row % 3 == 0) {
                UISpacer* spacer = UISpacerCreate(&gridpanel->e, 0, 1, 10);
        }
        UIPanel* rowpanel = UIPanelCreate(&gridpanel->e, UI_PANEL_COLOR_1 | UI_PANEL_MEDIUM_SPACING | UI_PANEL_HORIZONTAL);
        for (int col = 0; col < 9; col++) {
            if (col % 3 == 0) {
                UISpacer* spacer = UISpacerCreate(&rowpanel->e, 0, 10, 1);
            }
            textboxes.elements[row][col] = UITextboxCreate(&rowpanel->e, 0);
            textboxes.elements[row][col]->e.messageUser = TextboxMessage;
        }
    }
    UIPanel* buttonrow = UIPanelCreate(&panel->e, UI_PANEL_COLOR_1 | UI_PANEL_MEDIUM_SPACING | UI_PANEL_HORIZONTAL);
    UIButton* solver_button = UIButtonCreate(&buttonrow->e, 0, "Solve", -1);
    solver_button->e.messageUser = ButtonMessage;

    UIButton* clear_button = UIButtonCreate(&buttonrow->e, 0, "Clear", -1);
    clear_button->e.messageUser = ClearButtonMessage;

    UILabel* message_label = UILabelCreate(&panel->e, 0, "", -1);

    Context ctx = {
        .label = message_label,
        .textboxes = textboxes,
    };
    solver_button->e.cp = (void*)&ctx;
    clear_button->e.cp =(void*)&ctx;

    return UIMessageLoop();
    return 0;
}
