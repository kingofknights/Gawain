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
inline int Position::getLastTradeQuantity() const { return _lastTradeQuantity; }
inline int Position::getLastTradePrice() const { return _lastTradePrice; }
inline int Position::getTotalTradeQuantity() const { return _totalTradeQuantity; }

inline void Position::setLastTradeQuantity(int lastTradeQuantity_) { _lastTradeQuantity = lastTradeQuantity_; }
inline void Position::setLastTradePrice(int lastTradePrice_) { _lastTradePrice = lastTradePrice_; }
inline void Position::setTotalTradeQuantity(int totalTradeQuantity_) { _totalTradeQuantity = totalTradeQuantity_; }

//--------------------------------------------------------------------------------------
Internal::Internal() : _resultSetPtr(nullptr), _uniqueClassIdentity(++_globalStockPacketUniqueIdentifierCounter), _userAllocationPtr(nullptr) {}
Internal::~Internal() { delete _userAllocationPtr; }

inline int      Internal::getUniqueClassIdentity() const { return _uniqueClassIdentity; }
inline uint32_t Internal::getStrategyNumber() const { return _strategyNumber; }

inline ResultSetPtrT         Internal::getResultSetPtr() const { return _resultSetPtr; }
inline const AdaptorPtrT&    Internal::getAdaptorPtr() const { return _adaptorPtr; }
inline const StrategyPtrT&   Internal::getStrategyPtr() const { return _strategyPtr; }
inline CustomUserAllocation* Internal::getUserAllocationPtr() const { return _userAllocationPtr; }

inline void Internal::setStrategyNumber(uint32_t strategyNumber_) { _strategyNumber = strategyNumber_; }
inline void Internal::setResultSetPtr(const ResultSetT* resultSetPtr_) { _resultSetPtr = resultSetPtr_; }
inline void Internal::setAdaptorPtr(const AdaptorPtrT& adaptorPtr_) { _adaptorPtr = adaptorPtr_; }
inline void Internal::setStrategyPtr(const StrategyPtrT& strategyPtr_) { _strategyPtr = strategyPtr_; }
inline void Internal::setUserAllocationPtr(CustomUserAllocation* userAllocationPtr_) { _userAllocationPtr = userAllocationPtr_; }

//--------------------------------------------------------------------------------------
inline bool     OrderDetails::isIoc() const { return _ioc; }
inline Side     OrderDetails::getSide() const { return _side; }
inline int      OrderDetails::getPrice() const { return _price; }
inline uint32_t OrderDetails::getToken() const { return _token; }
inline int      OrderDetails::getQuantity() const { return _quantity; }
inline long     OrderDetails::getOrderNumber() const { return _orderNumber; }

inline std::string_view OrderDetails::getClientCode() const { return _clientCode; }
inline std::string_view OrderDetails::getAlgoCode() const { return _algoCode; }
inline std::string_view OrderDetails::getContractDescription() const { return _contractDescription; }

inline void OrderDetails::setSide(Side side_) { _side = side_; }
inline void OrderDetails::setIoc(bool ioc_) { _ioc = ioc_; }
inline void OrderDetails::setToken(uint32_t token_) { _token = token_; }
inline void OrderDetails::setPrice(int price_) { _price = price_; }
inline void OrderDetails::setQuantity(int quantity_) { _quantity = quantity_; }
inline void OrderDetails::setOrderNumber(long orderNumber_) { _orderNumber = orderNumber_; }
inline void OrderDetails::setClientCode(const std::string& clientCode_) { _clientCode = clientCode_; }
inline void OrderDetails::setAlgoCode(const std::string& algoCode_) { _algoCode = algoCode_; }
inline void OrderDetails::setContractDescription(const std::string& contractDescription_) { _contractDescription = contractDescription_; }

//--------------------------------------------------------------------------------------
StockPacket::StockPacket() : _lastRequest(OrderRequest_NONE), _currentStatus(OrderStatus_NONE), _previousStatus(OrderStatus_NONE) {}
inline void StockPacket::setLastRequest(OrderRequest lastRequest_) { _lastRequest = lastRequest_; }
inline void StockPacket::setCurrentStatus(OrderStatus currentStatus_) { _currentStatus = currentStatus_; }
inline void StockPacket::setPreviousStatus(OrderStatus previousStatus_) { _previousStatus = previousStatus_; }

inline OrderRequest StockPacket::getLastRequest() const { return _lastRequest; }
inline OrderStatus  StockPacket::getCurrentStatus() const { return _currentStatus; }
inline OrderStatus  StockPacket::getPreviousStatus() const { return _previousStatus; }

inline bool StockPacket::execute(int price_, int quantity_, OrderRequest request_) {
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

inline void StockPacket::executionReport(OrderStatus orderStatus_) {
    setPreviousStatus(getCurrentStatus());
    setCurrentStatus(orderStatus_);
    getStrategyPtr()->orderEventManager(getUniqueClassIdentity());
    getStrategyPtr()->updateArthur(shared_from_this());
}

}// namespace Lancelot::API