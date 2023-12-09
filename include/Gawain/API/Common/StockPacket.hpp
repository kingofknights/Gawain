//
// Created by kingofknights on 7/14/23.
//

#ifndef GAWAIN_INCLUDE_GAWAIN_API_COMMON_STOCK_PACKET_HPP_
#define GAWAIN_INCLUDE_GAWAIN_API_COMMON_STOCK_PACKET_HPP_
#pragma once

#include <memory>
#include <string>

namespace Lancelot {
enum Side : std::uint8_t;
struct ResultSetT;
using ResultSetPtrT = const ResultSetT*;

namespace API {
    enum OrderStatus : std::uint8_t;
    enum OrderRequest : std::uint8_t;

    class Strategy;
    class Adaptor;

    using StrategyPtrT = std::shared_ptr<Strategy>;
    using AdaptorPtrT  = std::shared_ptr<Adaptor>;

    //--------------------------------------------------------------------------------------
    class CustomUserAllocation {
      public:
        virtual ~CustomUserAllocation() = default;
    };

    //--------------------------------------------------------------------------------------
    class Position {
      public:
        [[nodiscard]] constexpr int getLastTradeQuantity() const;
        [[nodiscard]] constexpr int getLastTradePrice() const;
        [[nodiscard]] constexpr int getTotalTradeQuantity() const;

        constexpr void setLastTradeQuantity(int lastTradeQuantity_);
        constexpr void setLastTradePrice(int lastTradePrice_);
        constexpr void setTotalTradeQuantity(int totalTradeQuantity_);

      private:
        int _lastTradeQuantity  = 0;
        int _lastTradePrice     = 0;
        int _totalTradeQuantity = 0;
    };

    //--------------------------------------------------------------------------------------
    class Internal {
      public:
        [[nodiscard]] constexpr int      getUniqueClassIdentity() const;
        [[nodiscard]] constexpr uint32_t getStrategyNumber() const;

        [[nodiscard]] ResultSetPtrT         getResultSetPtr() const;
        [[nodiscard]] const AdaptorPtrT&    getAdaptorPtr() const;
        [[nodiscard]] const StrategyPtrT&   getStrategyPtr() const;
        [[nodiscard]] CustomUserAllocation* getUserAllocationPtr() const;

        Internal();
        ~Internal();

        constexpr void setStrategyNumber(uint32_t strategyNumber_);
        constexpr void setResultSetPtr(const ResultSetT* resultSetPtr_);
        void           setAdaptorPtr(const AdaptorPtrT& adaptorPtr_);
        void           setStrategyPtr(const StrategyPtrT& strategyPtr_);
        constexpr void setUserAllocationPtr(CustomUserAllocation* userAllocationPtr_);

      private:
        int      _uniqueClassIdentity = 0;
        uint32_t _strategyNumber      = 0;

        ResultSetPtrT         _resultSetPtr;
        AdaptorPtrT           _adaptorPtr;
        StrategyPtrT          _strategyPtr;
        CustomUserAllocation* _userAllocationPtr;
    };

    //--------------------------------------------------------------------------------------
    class OrderDetails {
      public:
        [[nodiscard]] constexpr bool     isIoc() const;
        [[nodiscard]] constexpr Side     getSide() const;
        [[nodiscard]] constexpr uint32_t getToken() const;
        [[nodiscard]] constexpr int      getPrice() const;
        [[nodiscard]] constexpr int      getQuantity() const;
        [[nodiscard]] constexpr long     getOrderNumber() const;

        [[nodiscard]] std::string_view getClientCode() const;
        [[nodiscard]] std::string_view getAlgoCode() const;
        [[nodiscard]] std::string_view getContractDescription() const;

        constexpr void setToken(uint32_t token_);
        constexpr void setSide(Side side_);
        constexpr void setPrice(int price_);
        constexpr void setQuantity(int quantity_);
        constexpr void setIoc(bool ioc_);
        constexpr void setOrderNumber(long orderNumber_);
        void           setClientCode(const std::string& clientCode_);
        void           setAlgoCode(const std::string& algoCode_);
        void           setContractDescription(const std::string& contractDescription_);

      private:
        bool     _ioc;
        Side     _side;
        int      _price;
        int      _quantity;
        uint32_t _token;
        long     _orderNumber;

        std::string _clientCode;
        std::string _algoCode;
        std::string _contractDescription;
    };

    //--------------------------------------------------------------------------------------
    class StockPacket final : public Position
        , public Internal
        , public OrderDetails
        , public std::enable_shared_from_this<StockPacket> {
      public:
        explicit StockPacket();
        [[nodiscard]] constexpr OrderRequest getLastRequest() const;
        [[nodiscard]] constexpr OrderStatus  getCurrentStatus() const;
        [[nodiscard]] constexpr OrderStatus  getPreviousStatus() const;

        constexpr void setLastRequest(OrderRequest lastRequest_);
        constexpr void setCurrentStatus(OrderStatus currentStatus_);
        constexpr void setPreviousStatus(OrderStatus previousStatus_);

        bool execute(int price_, int quantity_, OrderRequest request_);
        void executionReport(OrderStatus orderStatus_);

      private:
        OrderRequest _lastRequest;
        OrderStatus  _currentStatus;
        OrderStatus  _previousStatus;
    };

}// namespace API
}// namespace Lancelot

#endif// GAWAIN_INCLUDE_GAWAIN_API_COMMON_STOCK_PACKET_HPP_
