/*
 * Copyright (c) 2020, The SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/HTML/HTMLEmbedElement.h>

namespace Web::HTML {

HTMLEmbedElement::HTMLEmbedElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLEmbedElement::~HTMLEmbedElement()
{
}

}
