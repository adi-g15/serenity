/*
 * Copyright (c) 2020, The SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/HTML/HTMLQuoteElement.h>

namespace Web::HTML {

HTMLQuoteElement::HTMLQuoteElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLQuoteElement::~HTMLQuoteElement()
{
}

}
