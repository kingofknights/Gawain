//
// Created by kingofknights on 7/14/23.
//

#include "Gawain/API/Common/StockPacket.hpp"

#include "Gawain/API/Adaptor/Adaptor.hpp"
#include "Gawain/API/Common/Common.hpp"
#include "Gawain/API/Strategy/Strategy.hpp"

namespace Lancelot::API {

inline static int _globalStockPacketUniqueIdentifierCounter = 1;

//--------------------------------------------------------------------------------------
inline __attribute__((always_inline)) int Position::getLastTradeQuantity() const { return _lastTradeQuantity; }
inline __attribute__((always_inline)) int Position::getLastTradePrice() const { return _lastTradePrice; }
inline __attribute__((always_inline)) int Position::getTotalTradeQuantity() const { return _totalTradeQuantity; }

inline __attribute__((always_inline)) void Position::setLastTradeQuantity(int lastTradeQuantity_) { _lastTradeQuantity = lastTradeQuantity_; }
inline __attribute__((always_inline)) void Position::setLastTradePrice(int lastTradePrice_) { _lastTradePrice = lastTradePrice_; }
inline __attribute__((always_inline)) void Position::setTotalTradeQuantity(int totalTradeQuantity_) { _totalTradeQuantity = totalTradeQuantity_; }

//--------------------------------------------------------------------------------------
Internal::Internal() : _resultSetPtr(nullptr), _uniqueClassIdentity(++_globalStockPacketUniqueIdentifierCounter), _userAllocationPtr(nullptr) {}
Internal::~Internal() { delete _userAllocationPtr; }

inline __attribute__((always_inline)) int Internal::getUniqueClassIdentity() const { return _uniqueClassIdentity; }
inline __attribute__((always_inline)) int Internal::getStrategyNumber() const { return _strategyNumber; }

inline __attribute__((always_inline)) ResultSetPtrT         Internal::getResultSetPtr() const { return _resultSetPtr; }
inline __attribute__((always_inline)) const AdaptorPtrT&    Internal::getAdaptorPtr() const { return _adaptorPtr; }
inline __attribute__((always_inline)) const StrategyPtrT&   Internal::getStrategyPtr() const { return _strategyPtr; }
inline __attribute__((always_inline)) CustomUserAllocation* Internal::getUserAllocationPtr() const { return _userAllocationPtr; }

inline __attribute__((always_inline)) void Internal::setStrategyNumber(int strategyNumber_) { _strategyNumber = strategyNumber_; }
inline __attribute__((always_inline)) void Internal::setResultSetPtr(const ResultSetT* resultSetPtr_) { _resultSetPtr = resultSetPtr_; }
inline __attribute__((always_inline)) void Internal::setAdaptorPtr(const AdaptorPtrT& adaptorPtr_) { _adaptorPtr = adaptorPtr_; }
inline __attribute__((always_inline)) void Internal::setStrategyPtr(const StrategyPtrT& strategyPtr_) { _strategyPtr = strategyPtr_; }
inline __attribute__((always_inline)) void Internal::setUserAllocationPtr(CustomUserAllocation* userAllocationPtr_) { _userAllocationPtr = userAllocationPtr_; }

//--------------------------------------------------------------------------------------
inline __attribute__((always_inline)) bool     OrderDetails::isIoc() const { return _ioc; }
inline __attribute__((always_inline)) Side     OrderDetails::getSide() const { return _side; }
inline __attribute__((always_inline)) int      OrderDetails::getPrice() const { return _price; }
inline __attribute__((always_inline)) uint32_t OrderDetails::getToken() const { return _token; }
inline __attribute__((always_inline)) int      OrderDetails::getQuantity() const { return _quantity; }
inline __attribute__((always_inline)) long     OrderDetails::getOrderNumber() const { return _orderNumber; }

inline __attribute__((always_inline)) const std::string& OrderDetails::getClientCode() const { return _clientCode; }
inline __attribute__((always_inline)) const std::string& OrderDetails::getAlgoCode() const { return _algoCode; }
inline __attribute__((always_inline)) const std::string& OrderDetails::getContractDescription() const { return _contractDescription; }

inline __attribute__((always_inline)) void OrderDetails::setSide(Side side_) { _side = side_; }
inline __attribute__((always_inline)) void OrderDetails::setIoc(bool ioc_) { _ioc = ioc_; }
inline __attribute__((always_inline)) void OrderDetails::setToken(uint32_t token_) { _token = token_; }
inline __attribute__((always_inline)) void OrderDetails::setPrice(int price_) { _price = price_; }
inline __attribute__((always_inline)) void OrderDetails::setQuantity(int quantity_) { _quantity = quantity_; }
inline __attribute__((always_inline)) void OrderDetails::setOrderNumber(long orderNumber_) { _orderNumber = orderNumber_; }
inline __attribute__((always_inline)) void OrderDetails::setClientCode(const std::string& clientCode_) { _clientCode = clientCode_; }
inline __attribute__((always_inline)) void OrderDetails::setAlgoCode(const std::string& algoCode_) { _algoCode = algoCode_; }
inline __attribute__((always_inline)) void OrderDetails::setContractDescription(const std::string& contractDescription_) { _contractDescription = contractDescription_; }

//--------------------------------------------------------------------------------------
inline __attribute__((always_inline)) StockPacket::StockPacket() : _lastRequest(OrderRequest_NONE), _currentStatus(OrderStatus_NONE), _previousStatus(OrderStatus_NONE) {}
inline __attribute__((always_inline)) void StockPacket::setLastRequest(OrderRequest lastRequest_) { _lastRequest = lastRequest_; }
inline __attribute__((always_inline)) void StockPacket::setCurrentStatus(OrderStatus currentStatus_) { _currentStatus = currentStatus_; }
inline __attribute__((always_inline)) void StockPacket::setPreviousStatus(OrderStatus previousStatus_) { _previousStatus = previousStatus_; }

inline __attribute__((always_inline)) OrderRequest StockPacket::getLastRequest() const { return _lastRequest; }
inline __attribute__((always_inline)) OrderStatus  StockPacket::getCurrentStatus() const { return _currentStatus; }
inline __attribute__((always_inline)) OrderStatus  StockPacket::getPreviousStatus() const { return _previousStatus; }

inline __attribute__((always_inline)) bool StockPacket::execute(int price_, int quantity_, OrderRequest request_) {
    if (not getAdaptorPtr()) {
        return false;
    }

    auto orderStatus = getCurrentStatus();
    setCurrentStatus(OrderStatus_PENDING);

    if (getAdaptorPtr()->execute(shared_from_this(), price_, quantity_, request_)) {
        setPrice(price_);
        setQuantity(quantity_);
        return true;
    }
    setCurrentStatus(orderStatus);
    return false;
}

inline __attribute__((always_inline)) void StockPacket::executionReport(OrderStatus orderStatus_) {
    setPreviousStatus(getCurrentStatus());
    setCurrentStatus(orderStatus_);
    getStrategyPtr()->orderEventManager(getUniqueClassIdentity());
    getStrategyPtr()->updateArthur(shared_from_this());
}

}// namespace Lancelot::API