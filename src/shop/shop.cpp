#include "shop.hpp"
#include "../rendering/screen.hpp"
#include "../util/color.hpp"
#include "../structures/archer.hpp"
#include "../input.hpp"

const int Shop::entry_base_x_offset_{20};
const int Shop::entry_base_y_offset_{20};
const int Shop::entry_y_margin_{20};


Shop::Shop(Screen* screen, SDL_Rect rect, bool visible)
    : AbstractScene(screen, screen->createSurface(rect.w, rect.h), rect, visible)
    , selected_structure_index_{-1}
    , font_{nSDL_LoadFont(NSDL_FONT_VGA, 0x33, 0x33, 0x33)}
{
    // Draw background
    SDL_FillRect(background_surface_, NULL, Colors::SHOP_BACKGROUND);
}

RReader<Shop> Shop::create(Screen* screen, SDL_Rect rect, bool visible) {
    return screen->initScene(ROwner(new Shop(screen, rect, visible)));
}

Shop::~Shop() {
    nSDL_FreeFont(font_);
}

void Shop::render(RReader<Screen> screen) {
    auto surface = screen->getSurface();
    SDL_SetClipRect(surface, &rect_on_screen_);
    SDL_BlitSurface(background_surface_, NULL, surface, &rect_on_screen_);

    // Render all available structure
    // TODO: only in structure selection
    for (auto& entry : structure_entries_) {
        entry->display->render(surface);
    }

    if (selected_structure_index_ >= 0) {
        auto& selected_entry = structure_entries_.at(selected_structure_index_);

        // Render purchase outline
        auto bounding_box = selected_entry->display->boundingBox().resize(6, 6).centerOn(entry_base_x_offset_, entry_base_y_offset_ + selected_structure_index_ * entry_y_margin_);
        DrawUtils::drawRect(surface, bounding_box, Colors::SHOP_CURSOR, 1);

        nSDL_DrawString(surface, font_, 60, 20, selected_entry->name);
        nSDL_DrawString(surface, font_, 60, 40, "%i$", selected_entry->cost);
        nSDL_DrawString(surface, font_, 60, 60, selected_entry->details);
    }

    SDL_SetClipRect(surface, NULL);
}

RReader<StructureEntry> Shop::getSelectedStructure() const {
    return structure_entries_.at(selected_structure_index_).makeReader();
}

void Shop::applyUserActions(int input) {
    if (input & Input::UP && selected_structure_index_ > 0) selected_structure_index_ --;
    else if (input & Input::DOWN && (long int) selected_structure_index_ < (long int) structure_entries_.size() - 1) selected_structure_index_ ++;
}

RReader<StructureEntry> Shop::addStructureEntry(ROwner<StructureEntry>&& entry) {
    int y_offset = entry_base_y_offset_ + structure_entries_.size() * entry_y_margin_;
    entry->display->centerOn(entry_base_x_offset_, y_offset);
    structure_entries_.push_back(std::move(entry));
    if (selected_structure_index_ < 0) selected_structure_index_ = 0; // Ensure a valid entry is selected once becomes is available
    return structure_entries_.back().makeReader();
}
