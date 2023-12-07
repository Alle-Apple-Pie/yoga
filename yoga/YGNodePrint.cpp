/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifdef DEBUG

#include <stdarg.h>

#include <yoga/YGEnums.h>

#include "YGNodePrint.h"
#include "YGNode.h"
#include "Yoga-internal.h"
#include "Utils.h"

namespace facebook {
namespace yoga {
typedef std::string string;

static void indent(string& base, uint32_t level) {
  for (uint32_t i = 0; i < level; ++i) {
    base.append("  ");
  }
}

static bool areFourValuesEqual(const FBYGStyle::Edges& four) {
  return FBYGValueEqual(four[0], four[1]) && FBYGValueEqual(four[0], four[2]) &&
      FBYGValueEqual(four[0], four[3]);
}

static void appendFormattedString(string& str, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  va_list argsCopy;
  va_copy(argsCopy, args);
  std::vector<char> buf(1 + vsnprintf(NULL, 0, fmt, args));
  va_end(args);
  vsnprintf(buf.data(), buf.size(), fmt, argsCopy);
  va_end(argsCopy);
  string result = string(buf.begin(), buf.end() - 1);
  str.append(result);
}

static void appendFloatOptionalIfDefined(
    string& base,
    const string key,
    const FBYGFloatOptional num) {
  if (!num.isUndefined()) {
    appendFormattedString(base, "%s: %g; ", key.c_str(), num.unwrap());
  }
}

static void appendNumberIfNotUndefined(
    string& base,
    const string key,
    const FBYGValue number) {
  if (number.unit != FBYGUnitUndefined) {
    if (number.unit == FBYGUnitAuto) {
      base.append(key + ": auto; ");
    } else {
      string unit = number.unit == FBYGUnitPoint ? "px" : "%%";
      appendFormattedString(
          base, "%s: %g%s; ", key.c_str(), number.value, unit.c_str());
    }
  }
}

static void appendNumberIfNotAuto(
    string& base,
    const string& key,
    const FBYGValue number) {
  if (number.unit != FBYGUnitAuto) {
    appendNumberIfNotUndefined(base, key, number);
  }
}

static void appendNumberIfNotZero(
    string& base,
    const string& str,
    const FBYGValue number) {
  if (number.unit == FBYGUnitAuto) {
    base.append(str + ": auto; ");
  } else if (!FBYGFloatsEqual(number.value, 0)) {
    appendNumberIfNotUndefined(base, str, number);
  }
}

static void appendEdges(
    string& base,
    const string& key,
    const FBYGStyle::Edges& edges) {
  if (areFourValuesEqual(edges)) {
    auto edgeValue = FBYGNode::computeEdgeValueForColumn(
        edges, FBYGEdgeLeft, detail::CompactValue::ofZero());
    appendNumberIfNotZero(base, key, edgeValue);
  } else {
    for (int edge = FBYGEdgeLeft; edge != FBYGEdgeAll; ++edge) {
      string str = key + "-" + FBYGEdgeToString(static_cast<FBYGEdge>(edge));
      appendNumberIfNotZero(base, str, edges[edge]);
    }
  }
}

static void appendEdgeIfNotUndefined(
    string& base,
    const string& str,
    const FBYGStyle::Edges& edges,
    const FBYGEdge edge) {
  // TODO: this doesn't take RTL / FBYGEdgeStart / FBYGEdgeEnd into account
  auto value = (edge == FBYGEdgeLeft || edge == FBYGEdgeRight)
      ? FBYGNode::computeEdgeValueForRow(
            edges, edge, edge, detail::CompactValue::ofUndefined())
      : FBYGNode::computeEdgeValueForColumn(
            edges, edge, detail::CompactValue::ofUndefined());
  appendNumberIfNotUndefined(base, str, value);
}

void FBYGNodeToString(
    std::string& str,
    FBYGNodeRef node,
    FBYGPrintOptions options,
    uint32_t level) {
  indent(str, level);
  appendFormattedString(str, "<div ");

  if (options & FBYGPrintOptionsLayout) {
    appendFormattedString(str, "layout=\"");
    appendFormattedString(
        str, "width: %g; ", node->getLayout().dimensions[FBYGDimensionWidth]);
    appendFormattedString(
        str, "height: %g; ", node->getLayout().dimensions[FBYGDimensionHeight]);
    appendFormattedString(
        str, "top: %g; ", node->getLayout().position[FBYGEdgeTop]);
    appendFormattedString(
        str, "left: %g;", node->getLayout().position[FBYGEdgeLeft]);
    appendFormattedString(str, "\" ");
  }

  if (options & FBYGPrintOptionsStyle) {
    appendFormattedString(str, "style=\"");
    const auto& style = node->getStyle();
    if (style.flexDirection() != FBYGNode().getStyle().flexDirection()) {
      appendFormattedString(
          str,
          "flex-direction: %s; ",
          FBYGFlexDirectionToString(style.flexDirection()));
    }
    if (style.justifyContent() != FBYGNode().getStyle().justifyContent()) {
      appendFormattedString(
          str,
          "justify-content: %s; ",
          FBYGJustifyToString(style.justifyContent()));
    }
    if (style.alignItems() != FBYGNode().getStyle().alignItems()) {
      appendFormattedString(
          str, "align-items: %s; ", FBYGAlignToString(style.alignItems()));
    }
    if (style.alignContent() != FBYGNode().getStyle().alignContent()) {
      appendFormattedString(
          str, "align-content: %s; ", FBYGAlignToString(style.alignContent()));
    }
    if (style.alignSelf() != FBYGNode().getStyle().alignSelf()) {
      appendFormattedString(
          str, "align-self: %s; ", FBYGAlignToString(style.alignSelf()));
    }
    appendFloatOptionalIfDefined(str, "flex-grow", style.flexGrow());
    appendFloatOptionalIfDefined(str, "flex-shrink", style.flexShrink());
    appendNumberIfNotAuto(str, "flex-basis", style.flexBasis());
    appendFloatOptionalIfDefined(str, "flex", style.flex());

    if (style.flexWrap() != FBYGNode().getStyle().flexWrap()) {
      appendFormattedString(
          str, "flex-wrap: %s; ", FBYGWrapToString(style.flexWrap()));
    }

    if (style.overflow() != FBYGNode().getStyle().overflow()) {
      appendFormattedString(
          str, "overflow: %s; ", FBYGOverflowToString(style.overflow()));
    }

    if (style.display() != FBYGNode().getStyle().display()) {
      appendFormattedString(
          str, "display: %s; ", FBYGDisplayToString(style.display()));
    }
    appendEdges(str, "margin", style.margin());
    appendEdges(str, "padding", style.padding());
    appendEdges(str, "border", style.border());

    if (FBYGNode::computeColumnGap(
            style.gap(), detail::CompactValue::ofUndefined()) !=
        FBYGNode::computeColumnGap(
            FBYGNode().getStyle().gap(), detail::CompactValue::ofUndefined())) {
      appendNumberIfNotUndefined(
          str, "column-gap", style.gap()[FBYGGutterColumn]);
    }
    if (FBYGNode::computeRowGap(
            style.gap(), detail::CompactValue::ofUndefined()) !=
        FBYGNode::computeRowGap(
            FBYGNode().getStyle().gap(), detail::CompactValue::ofUndefined())) {
      appendNumberIfNotUndefined(str, "row-gap", style.gap()[FBYGGutterRow]);
    }

    appendNumberIfNotAuto(str, "width", style.dimensions()[FBYGDimensionWidth]);
    appendNumberIfNotAuto(str, "height", style.dimensions()[FBYGDimensionHeight]);
    appendNumberIfNotAuto(
        str, "max-width", style.maxDimensions()[FBYGDimensionWidth]);
    appendNumberIfNotAuto(
        str, "max-height", style.maxDimensions()[FBYGDimensionHeight]);
    appendNumberIfNotAuto(
        str, "min-width", style.minDimensions()[FBYGDimensionWidth]);
    appendNumberIfNotAuto(
        str, "min-height", style.minDimensions()[FBYGDimensionHeight]);

    if (style.positionType() != FBYGNode().getStyle().positionType()) {
      appendFormattedString(
          str, "position: %s; ", FBYGPositionTypeToString(style.positionType()));
    }

    appendEdgeIfNotUndefined(str, "left", style.position(), FBYGEdgeLeft);
    appendEdgeIfNotUndefined(str, "right", style.position(), FBYGEdgeRight);
    appendEdgeIfNotUndefined(str, "top", style.position(), FBYGEdgeTop);
    appendEdgeIfNotUndefined(str, "bottom", style.position(), FBYGEdgeBottom);
    appendFormattedString(str, "\" ");

    if (node->hasMeasureFunc()) {
      appendFormattedString(str, "has-custom-measure=\"true\"");
    }
  }
  appendFormattedString(str, ">");

  const uint32_t childCount = static_cast<uint32_t>(node->getChildren().size());
  if (options & FBYGPrintOptionsChildren && childCount > 0) {
    for (uint32_t i = 0; i < childCount; i++) {
      appendFormattedString(str, "\n");
      FBYGNodeToString(str, FBYGNodeGetChild(node, i), options, level + 1);
    }
    appendFormattedString(str, "\n");
    indent(str, level);
  }
  appendFormattedString(str, "</div>");
}
} // namespace yoga
} // namespace facebook
#endif
