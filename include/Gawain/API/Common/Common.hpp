//
// Created by kingofknights on 7/14/23.
//

#ifndef GAWAIN_INCLUDE_LANCELOT_API_COMMON_HPP_
#define GAWAIN_INCLUDE_LANCELOT_API_COMMON_HPP_
#pragma once

#include <string>
namespace Lancelot::API {
enum OrderType : std::uint8_t {
    OrderType_LIMIT = 0,
    OrderType_MARKET,
    OrderType_IOC,
    OrderType_SPREAD
};

enum StrategyStatus : std::uint8_t {
    StrategyStatus_PENDING = 0,
    StrategyStatus_ACTIVE,
    StrategyStatus_APPLIED,
    StrategyStatus_INACTIVE,
    StrategyStatus_TERMINATED,
    StrategyStatus_WAITING,
    StrategyStatus_DISCONNECTED
};

enum OrderStatus : std::uint8_t {
    OrderStatus_NONE = 0,
    OrderStatus_PLACED,
    OrderStatus_NEW,
    OrderStatus_REPLACED,
    OrderStatus_CANCELLED,
    OrderStatus_NEW_REJECT,
    OrderStatus_REPLACE_REJECT,
    OrderStatus_CANCEL_REJECT,
    OrderStatus_PARTIAL_FILLED,
    OrderStatus_FILLED,
    OrderStatus_PENDING,
    OrderStatus_ADAPTOR_REJECT
};

enum OrderRequest : std::uint8_t {
    OrderRequest_NONE = 0,
    OrderRequest_NEW,
    OrderRequest_MODIFY,
    OrderRequest_CANCEL,
    OrderRequest_IOC,
    OrderRequest_CANCEL_ALL
};

std::string toString(OrderType orderType_);
std::string toString(StrategyStatus status_);
std::string toString(OrderStatus status_);
std::string toString(OrderRequest request_);
}// namespace Lancelot::API
#endif// GAWAIN_INCLUDE_LANCELOT_API_COMMON_HPP_
