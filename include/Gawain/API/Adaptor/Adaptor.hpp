//
// Created by kingofknights on 7/15/23.
//

#ifndef GAWAIN_INCLUDE_LANCELOT_API_ADAPTOR_ADAPTOR_HPP_
#define GAWAIN_INCLUDE_LANCELOT_API_ADAPTOR_ADAPTOR_HPP_
#pragma once

#include <memory>
#include <thread>
#include <vector>

namespace Lancelot {
using ThreadPointerT = std::unique_ptr<std::jthread>;
using ThreadGroupT   = std::vector<ThreadPointerT>;

enum Exchange : std::uint8_t;
namespace API {
    enum OrderRequest : std::uint8_t;
    enum OrderStatus : std::uint8_t;

    class StockPacket;
    using StockPacketPtrT = std::shared_ptr<StockPacket>;

    class Adaptor {
      public:
        virtual ~Adaptor() = default;

        virtual void initialization(ThreadGroupT& threadGroup_) = 0;

        virtual void forwardAssemble(const StockPacketPtrT& order_) = 0;

        virtual bool execute(const StockPacketPtrT& order_, int price_, int quantity_, OrderRequest request_) = 0;

        static void OrderResponse(const StockPacketPtrT& order_, OrderStatus status_);

        static void OnDisconnection(Exchange exchange_);

        static void OnConnection(Exchange exchange_);
    };
}// namespace API
}// namespace Lancelot
#endif// GAWAIN_INCLUDE_LANCELOT_API_ADAPTOR_ADAPTOR_HPP_
