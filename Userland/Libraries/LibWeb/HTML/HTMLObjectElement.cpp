/*
 * Copyright (c) 2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibGfx/Bitmap.h>
#include <LibWeb/CSS/StyleResolver.h>
#include <LibWeb/DOM/Document.h>
#include <LibWeb/DOM/Event.h>
#include <LibWeb/HTML/HTMLObjectElement.h>
#include <LibWeb/Layout/ImageBox.h>
#include <LibWeb/Loader/ResourceLoader.h>

namespace Web::HTML {

HTMLObjectElement::HTMLObjectElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
    , m_image_loader(*this)
{
    m_image_loader.on_load = [this] {
        m_should_show_fallback_content = false;
        this->document().force_layout();
    };

    m_image_loader.on_fail = [this] {
        m_should_show_fallback_content = true;
        this->document().force_layout();
    };
}

HTMLObjectElement::~HTMLObjectElement()
{
}

void HTMLObjectElement::parse_attribute(const FlyString& name, const String& value)
{
    HTMLElement::parse_attribute(name, value);

    if (name == HTML::AttributeNames::data)
        m_image_loader.load(document().complete_url(value));
}

RefPtr<Layout::Node> HTMLObjectElement::create_layout_node()
{
    if (m_should_show_fallback_content)
        return HTMLElement::create_layout_node();

    auto style = document().style_resolver().resolve_style(*this);
    if (style->display() == CSS::Display::None)
        return nullptr;
    if (m_image_loader.has_image())
        return adopt(*new Layout::ImageBox(document(), *this, move(style), m_image_loader));
    return nullptr;
}

}
