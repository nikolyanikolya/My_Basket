#include "additional_windows.h"
#include <QBrush>
#include <QGridLayout>
#include <QLabel>
#include <QPalette>
#include <QSizePolicy>
#include <iostream>
#include "API.h"
#include "style_settings.h"

namespace Ui {

void set_background_image(QWidget *window,
                          std::string const &path_to_bg_image) {
    QBrush image_basket_background(QImage(path_to_bg_image.c_str()));
    QPalette plt = window->palette();
    plt.setBrush(QPalette::Window, image_basket_background);
    window->setPalette(plt);
}

void set_minimum_window_sizes(QWidget *window, int width, int height) {
    window->setMinimumSize(width, height);
}

void set_font_color_button(QPushButton *button,
                           std::string const &bg_color,
                           int font_size,
                           int enabled,
                           std::string const &font) {
    std::string s = "QPushButton { background-color : " + bg_color +
                    "; color : white; border-width: 5px; border-radius: 10px; "
                    "padding: 6px; font-size: " +
                    std::to_string(font_size) + "px;";
    if (!font.empty()) {
        s += " font: " + font;
    }
    s += "}";
    button->setStyleSheet(s.c_str());

    if (enabled != -1) {
        button->setEnabled(static_cast<bool>(enabled));
    }
}
////в качестве параметра поставлять структуру настройки

void set_font_color_label(QLabel *label,
                          std::string const &word_color,
                          int font_size,
                          std::string const &font,
                          std::string const &bg_color) {
    std::string s = "QLabel { color : " + word_color +
                    "; border-width: "
                    "2px; border-radius: 5px; padding: 2px; font-size: " +
                    std::to_string(font_size) + "px";
    if (!font.empty()) {
        s += "; font: " + font + "; ";
    }
    if (!bg_color.empty()) {
        s += "; background-color : " + bg_color + ";";
    }
    s += "}";
    label->setStyleSheet(s.c_str());
}

static void print_products_vector_with_costs(
    QTextEdit *text_field,
    std::vector<std::pair<std::string, uint32_t>> const &products_vec) {
    text_field->insertPlainText(
        (StyleSettings::Titles::shop_list_title + "\n\n").c_str());
    int vec_sz = products_vec.size();
    for (int i = 0; i < vec_sz; i++) {
        std::string s = std::to_string(i + 1) + ") ";
        text_field->insertPlainText(s.c_str());

        s = products_vec[i].first + "\n<--> " +
            std::to_string(products_vec[i].second) + "₽\n\n";
        text_field->insertPlainText(s.c_str());
    }
    ///выглядит, как копипаст предыдущей функции, пофиксить
}

void button_showed(QPushButton *button, bool is_showed) {
    if (is_showed) {
        button->show();
    } else {
        button->hide();
    }
}

StartWindow::StartWindow(QWidget *parent) : QWidget(parent) {
    start_shopping_button = new QPushButton(
        StyleSettings::Titles::start_shopping_button_title.c_str());
    set_font_color_button(
        start_shopping_button, StyleSettings::Colors::pink_medium_button,
        StyleSettings::FontSizes::start_shopping_button_fz, true);
    start_shopping_button->show();

    my_basket_label = new QLabel(StyleSettings::Titles::windows_title.c_str());
    set_font_color_label(my_basket_label, StyleSettings::Colors::black,
                         StyleSettings::FontSizes::my_basket_label_fz,
                         StyleSettings::FontStyles::bold);
    my_basket_label->setAlignment(Qt::AlignCenter);
    my_basket_label->setMargin(
        StyleSettings::WindowSizes::my_basket_label_margin);

    description_label =
        new QLabel(StyleSettings::Titles::description_label_title.c_str());
    set_font_color_label(description_label, StyleSettings::Colors::black,
                         StyleSettings::FontSizes::description_label_fz);
    description_label->setAlignment(Qt::AlignCenter);

    auto *label_layout = new QVBoxLayout;
    label_layout->addWidget(my_basket_label, Qt::AlignTop, Qt::AlignVCenter);
    label_layout->addWidget(description_label, Qt::AlignTop, Qt::AlignVCenter);
    label_layout->setSpacing(
        StyleSettings::WindowSizes::start_window_layout_spacing);

    auto *main_layout = new QVBoxLayout;
    main_layout->addLayout(label_layout);
    main_layout->addWidget(start_shopping_button, Qt::AlignTop,
                           Qt::AlignVCenter);
    main_layout->setSpacing(
        StyleSettings::WindowSizes::start_window_layout_spacing);

    connect(start_shopping_button, SIGNAL(clicked()), this,
            SLOT(go_to_category_window()));

    //параметры окна
    setLayout(main_layout);
    setWindowTitle(StyleSettings::Titles::windows_title.c_str());
    set_background_image(this);
    set_minimum_window_sizes(this);
}

void StartWindow::go_to_category_window() {
    set_font_color_button(
        start_shopping_button, StyleSettings::Colors::pink_hard_button,
        StyleSettings::FontSizes::start_shopping_button_fz, false);

    //переход к recipe_book
    auto category_window = new CategoryWindow();
    category_window->show();
    this->close();
}

void CategoryWindow::button_enabled(QPushButton *button, bool is_enabled) {
    if (is_enabled) {
        set_font_color_button(
            button, StyleSettings::Colors::pink_medium_button,
            StyleSettings::FontSizes::category_window_buttons_sz, true);
    } else {
        set_font_color_button(
            button, StyleSettings::Colors::pink_hard_button,
            StyleSettings::FontSizes::category_window_buttons_sz, false);
    }
}

void CategoryWindow::update_buttons_enabled(bool economy_button_enabled,
                                            bool base_button_enabled,
                                            bool premium_button_enabled) {
    button_enabled(economy_button, economy_button_enabled);
    button_enabled(base_button, base_button_enabled);
    button_enabled(premium_button, premium_button_enabled);
}

CategoryWindow::CategoryWindow(QWidget *parent) : QWidget(parent) {
    choose_category_label =
        new QLabel(StyleSettings::Titles::choose_category_label_title.c_str());
    set_font_color_label(choose_category_label, StyleSettings::Colors::black,
                         StyleSettings::FontSizes::choose_category_label_sz);
    choose_category_label->setAlignment(Qt::AlignCenter);
    choose_category_label->setMargin(
        StyleSettings::WindowSizes::choose_category_label_margin);

    economy_button =
        new QPushButton(StyleSettings::Titles::economy_button_title.c_str());
    economy_button->show();

    base_button =
        new QPushButton(StyleSettings::Titles::base_button_title.c_str());
    base_button->show();

    premium_button =
        new QPushButton(StyleSettings::Titles::premium_button_title.c_str());
    premium_button->show();

    //установка настроек кнопок категорий
    update_buttons_enabled();

    go_to_searching_button =
        new QPushButton(StyleSettings::Titles::go_to_searching_button.c_str());
    set_font_color_button(
        go_to_searching_button, StyleSettings::Colors::pink_light_button,
        StyleSettings::FontSizes::category_window_buttons_sz, false);
    go_to_searching_button->show();

    auto *button_layout = new QHBoxLayout;
    button_layout->addWidget(economy_button, Qt::AlignTop, Qt::AlignVCenter);
    button_layout->addWidget(base_button, Qt::AlignTop, Qt::AlignVCenter);
    button_layout->addWidget(premium_button, Qt::AlignTop, Qt::AlignVCenter);

    auto *main_layout = new QVBoxLayout;
    main_layout->addWidget(choose_category_label, Qt::AlignTop);
    main_layout->addLayout(button_layout);
    main_layout->addWidget(go_to_searching_button, Qt::AlignTop,
                           Qt::AlignVCenter);
    main_layout->setSpacing(StyleSettings::WindowSizes::cw_main_layout_spacing);

    connect(economy_button, SIGNAL(clicked()), this, SLOT(choose_economy()));
    connect(base_button, SIGNAL(clicked()), this, SLOT(choose_base()));
    connect(premium_button, SIGNAL(clicked()), this, SLOT(choose_premium()));
    connect(go_to_searching_button, SIGNAL(clicked()), this,
            SLOT(go_to_recipe_book()));

    //параметры окна
    setLayout(main_layout);
    setWindowTitle(StyleSettings::Titles::windows_title.c_str());
    set_background_image(this);
    set_minimum_window_sizes(this);
}

void CategoryWindow::choose_economy() {
    chosen_category = StyleSettings::Titles::category_economy;

    update_buttons_enabled(false, true, true);

    set_font_color_button(
        go_to_searching_button, StyleSettings::Colors::pink_medium_button,
        StyleSettings::FontSizes::category_window_buttons_sz, true);
}

void CategoryWindow::choose_base() {
    chosen_category = StyleSettings::Titles::category_base;

    update_buttons_enabled(true, false, true);

    set_font_color_button(
        go_to_searching_button, StyleSettings::Colors::pink_medium_button,
        StyleSettings::FontSizes::category_window_buttons_sz, true);
}

void CategoryWindow::choose_premium() {
    chosen_category = StyleSettings::Titles::category_premium;

    update_buttons_enabled(true, true, false);

    set_font_color_button(
        go_to_searching_button, StyleSettings::Colors::pink_medium_button,
        StyleSettings::FontSizes::category_window_buttons_sz, true);
}

void CategoryWindow::go_to_recipe_book() {
    set_font_color_button(
        go_to_searching_button, StyleSettings::Colors::pink_hard_button,
        StyleSettings::FontSizes::category_window_buttons_sz, false);
    API::ingredients_and_recipes::choose_category_shop(chosen_category);

    //переход к recipe_book
    auto recipe_book = new RecipeBook();
    recipe_book->show();
    this->close();
}

SummaryWindow::SummaryWindow(QWidget *parent) : QWidget(parent) {
    //получение информации о лучшем магазине и лучшей стоимости
    auto calculation_info =
        API::ingredients_and_recipes::compare_prices_of_ingredients();
    shop_name = calculation_info.first.first;
    total_cost = calculation_info.first.second;
    products_vec = calculation_info.second;

    best_total_cost_label =
        new QLabel(StyleSettings::Titles::best_total_cost_label_title.c_str());
    set_font_color_label(
        best_total_cost_label, StyleSettings::Colors::black,
        StyleSettings::FontSizes::summary_window_labels_text_sz);
    best_total_cost_label->setAlignment(Qt::AlignCenter);
    best_total_cost_label->setMargin(
        StyleSettings::WindowSizes::summary_window_margin);

    total_cost_number_label =
        new QLabel((std::to_string(total_cost) + "₽").c_str());
    set_font_color_label(
        total_cost_number_label, StyleSettings::Colors::black,
        StyleSettings::FontSizes::summary_window_labels_text_sz,
        StyleSettings::FontStyles::bold);
    total_cost_number_label->setAlignment(Qt::AlignCenter);
    total_cost_number_label->setMargin(
        StyleSettings::WindowSizes::summary_window_margin);

    shop_name_label = new QLabel(shop_name.c_str());
    set_font_color_label(
        shop_name_label, StyleSettings::Colors::black,
        StyleSettings::FontSizes::summary_window_labels_text_sz,
        StyleSettings::FontStyles::bold);
    shop_name_label->setAlignment(Qt::AlignCenter);
    shop_name_label->setMargin(
        StyleSettings::WindowSizes::summary_window_margin);

    in_shop_label =
        new QLabel(StyleSettings::Titles::in_shop_label_title.c_str());
    set_font_color_label(
        in_shop_label, StyleSettings::Colors::black,
        StyleSettings::FontSizes::summary_window_labels_text_sz);
    in_shop_label->setAlignment(Qt::AlignCenter);
    in_shop_label->setMargin(StyleSettings::WindowSizes::summary_window_margin);

    products_text = new QTextEdit;
    products_text->setFontPointSize(
        StyleSettings::FontSizes::summary_window_products_text_sz);
    products_text->setFixedSize(
        StyleSettings::WindowSizes::product_text_width,
        StyleSettings::WindowSizes::product_text_height);
    products_text->setReadOnly(true);
    products_text->hide();

    //кнопки
    show_final_products_button = new QPushButton(
        StyleSettings::Titles::show_final_products_button_title.c_str());
    set_font_color_button(
        show_final_products_button, StyleSettings::Colors::pink_medium_button,
        StyleSettings::FontSizes::summary_window_medium_buttons_sz);
    show_final_products_button->show();

    start_again_button = new QPushButton(
        StyleSettings::Titles::start_again_button_title.c_str());
    set_font_color_button(
        start_again_button, StyleSettings::Colors::pink_medium_button,
        StyleSettings::FontSizes::summary_window_medium_buttons_sz);
    start_again_button->show();

    show_check_button =
        new QPushButton(StyleSettings::Titles::show_check_button_title.c_str());
    set_font_color_button(
        show_check_button, StyleSettings::Colors::pink_medium_button,
        StyleSettings::FontSizes::summary_window_medium_buttons_sz);
    show_check_button->hide();

    end_program_button = new QPushButton(
        StyleSettings::Titles::end_program_button_title.c_str());
    set_font_color_button(
        end_program_button, StyleSettings::Colors::pink_medium_button,
        StyleSettings::FontSizes::summary_window_medium_buttons_sz);
    end_program_button->show();

    QGridLayout *label_layout = new QGridLayout;
    label_layout->addWidget(best_total_cost_label);
    label_layout->addWidget(total_cost_number_label);
    label_layout->addWidget(in_shop_label);
    label_layout->addWidget(shop_name_label);
    label_layout->addWidget(products_text);
    label_layout->setSpacing(
        StyleSettings::WindowSizes::sw_label_layout_spacing);

    QHBoxLayout *button_layout = new QHBoxLayout;
    button_layout->addWidget(show_final_products_button);
    button_layout->addWidget(show_check_button);
    button_layout->addWidget(start_again_button);
    button_layout->addWidget(end_program_button);

    QGridLayout *main_layout = new QGridLayout;
    main_layout->addLayout(label_layout, 0, 0);
    main_layout->addLayout(button_layout, 1, 0);
    main_layout->setSpacing(StyleSettings::WindowSizes::sw_main_layout_spacing);

    connect(show_final_products_button, SIGNAL(clicked()), this,
            SLOT(show_final_products_func()));
    connect(show_check_button, SIGNAL(clicked()), this,
            SLOT(show_check_func()));
    connect(start_again_button, SIGNAL(clicked()), this,
            SLOT(start_again_func()));
    connect(end_program_button, SIGNAL(clicked()), this,
            SLOT(end_program_func()));

    //параметры окна
    setLayout(main_layout);
    setWindowTitle(StyleSettings::Titles::windows_title.c_str());
    set_background_image(this);
    set_minimum_window_sizes(this);
}

void SummaryWindow::show_final_products_func() {
    show_final_products_button->hide();
    show_check_button->show();

    best_total_cost_label->hide();
    total_cost_number_label->hide();
    in_shop_label->hide();
    shop_name_label->hide();

    products_text->show();
    print_products_vector_with_costs(products_text, products_vec);
}

void SummaryWindow::show_check_func() {
    show_final_products_button->show();
    show_check_button->hide();

    best_total_cost_label->show();
    total_cost_number_label->show();
    in_shop_label->show();
    shop_name_label->show();

    products_text->hide();
}

void SummaryWindow::start_again_func() {
    set_font_color_button(
        start_again_button, StyleSettings::Colors::pink_hard_button,
        StyleSettings::FontSizes::summary_window_medium_buttons_sz, false);

    //очистка векторов
    API::ingredients_and_recipes::discard_all();
    CategoryWindow *category_window = new CategoryWindow();
    category_window->show();
    this->close();
}

void SummaryWindow::end_program_func() {
    std::exit(0);
}

}  // namespace Ui
