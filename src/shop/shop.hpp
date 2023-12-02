#pragma once
#include <vector>
#include "../rendering/abstract_scene.hpp"
#include "../structures/structure.hpp"
#include "../structures/tower.hpp"
#include "../util/event.hpp"

enum class PurchaseType {
    STRUCTURE,
    UPGRADE,
};

struct ShopEntry {
    ShopEntry(PurchaseType purchase_type_, int cost_, ROwner<RendererObject>&& display_, const char* name_, const char* details_)
        : purchase_type{purchase_type_}
        , cost{cost_}
        , display{std::move(display_)}
        , name{name_}
        , details{details_}
        {};
    PurchaseType purchase_type;
    int cost;
    ROwner<RendererObject> display;
    const char* name;
    const char* details;
};


struct StructureEntry : public ShopEntry {
    StructureEntry(int cost_, ROwner<RendererObject>&& display_, const char* name_, const char* details_, StructureType structure_type_)
        : ShopEntry(PurchaseType::STRUCTURE, cost_, std::move(display_), name_, details_)
        , structure_type{structure_type_}
        {};
    StructureType structure_type;
};

struct TowerEntry : public StructureEntry {
    TowerEntry(int cost_, ROwner<RendererObject>&& display_, const char* name_, const char* details_, TowerType tower_type_)
        : StructureEntry(cost_, std::move(display_), name_, details_, StructureType::TOWER)
        , tower_type{tower_type_}
        {};

    TowerType tower_type;
};


class Shop : public AbstractScene {
public:
    static RReader<Shop> create(Screen* screen, SDL_Rect rect, bool visible);

    Shop(Shop&& other) = default;
    Shop& operator=(Shop&& other) = default;

    virtual ~Shop();

    /// @brief Render scene to the screen
    virtual void render(RReader<Screen> screen) override;

    /// @brief Add a shop entry for a purchaseable structure
    /// @param entry Entry to add
    RReader<StructureEntry> addStructureEntry(ROwner<StructureEntry>&& entry);

    /// @brief Get the selected structure the user selected in the shop
    /// @throws If the shop has no valid entries
    RReader<StructureEntry> getSelectedStructure() const;

    /// @brief Apply user input according to shop logic
    /// @param input Bit-wise OR-ed actions
    void applyUserActions(int input);

private:
    /// @note Interaction with screen is necessary for proper initialization, hence create() method.
    Shop(Screen* screen, SDL_Rect rect, bool visible = true);

    /// @brief Registered entries for purchaseable structures
    std::vector<ROwner<StructureEntry>> structure_entries_;

    /// @brief Structure selected in the shop.
    int selected_structure_index_;

    static const int entry_base_x_offset_;
    static const int entry_base_y_offset_;
    static const int entry_y_margin_;

    // Main font
    nSDL_Font* font_;
};
