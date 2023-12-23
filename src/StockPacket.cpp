//
// Created by kingofknights on 7/14/23.
//

#include "Gawain/API/Common/StockPacket.hpp"

#include "Gawain/API/Adaptor/Adaptor.hpp"
#include "Gawain/API/Common/Common.hpp"
#include "Gawain/API/Strategy/Strategy.hpp"

namespace Lancelot::API {

namespace {
    static int _globalStockPacketUniqueIdentifierCounter = 1;
}
//--------------------------------------------------------------------------------------
int Position::getLastTradeQuantity() const { return _lastTradeQuantity; }
int Position::getLastTradePrice() const { return _lastTradePrice; }
int Position::getTotalTradeQuantity() const { return _totalTradeQuantity; }

void Position::setLastTradeQuantity(int lastTradeQuantity_) { _lastTradeQuantity = lastTradeQuantity_; }
void Position::setLastTradePrice(int lastTradePrice_) { _lastTradePrice = lastTradePrice_; }
void Position::setTotalTradeQuantity(int totalTradeQuantity_) { _totalTradeQuantity = totalTradeQuantity_; }

//--------------------------------------------------------------------------------------
Internal::Internal() : _resultSetPtr(nullptr), _uniqueClassIdentity(++_globalStockPacketUniqueIdentifierCounter), _userAllocationPtr(nullptr) {}
Internal::~Internal() { delete _userAllocationPtr; }

int      Internal::getUniqueClassIdentity() const { return _uniqueClassIdentity; }
uint32_t Internal::getStrategyNumber() const { return _strategyNumber; }

ResultSetPtrT         Internal::getResultSetPtr() const { return _resultSetPtr; }
const AdaptorPtrT&    Internal::getAdaptorPtr() const { return _adaptorPtr; }
const StrategyPtrT&   Internal::getStrategyPtr() const { return _strategyPtr; }
CustomUserAllocation* Internal::getUserAllocationPtr() const { return _userAllocationPtr; }

void Internal::setStrategyNumber(uint32_t strategyNumber_) { _strategyNumber = strategyNumber_; }
void Internal::setResultSetPtr(const ResultSetT* resultSetPtr_) { _resultSetPtr = resultSetPtr_; }
void Internal::setAdaptorPtr(const AdaptorPtrT& adaptorPtr_) { _adaptorPtr = adaptorPtr_; }
void Internal::setStrategyPtr(const StrategyPtrT& strategyPtr_) { _strategyPtr = strategyPtr_; }
void Internal::setUserAllocationPtr(CustomUserAllocation* userAllocationPtr_) { _userAllocationPtr = userAllocationPtr_; }

//--------------------------------------------------------------------------------------
bool     OrderDetails::isIoc() const { return _ioc; }
Side     OrderDetails::getSide() const { return _side; }
int      OrderDetails::getPrice() const { return _price; }
uint32_t OrderDetails::getToken() const { return _token; }
int      OrderDetails::getQuantity() const { return _quantity; }
long     OrderDetails::getOrderNumber() const { return _orderNumber; }

std::string_view OrderDetails::getClientCode() const { return _clientCode; }
std::string_view OrderDetails::getAlgoCode() const { return _algoCode; }
std::string_view OrderDetails::getContractDescription() const { return _contractDescription; }

void OrderDetails::setSide(Side side_) { _side = side_; }
void OrderDetails::setIoc(bool ioc_) { _ioc = ioc_; }
void OrderDetails::setToken(uint32_t token_) { _token = token_; }
void OrderDetails::setPrice(int price_) { _price = price_; }
void OrderDetails::setQuantity(int quantity_) { _quantity = quantity_; }
void OrderDetails::setOrderNumber(long orderNumber_) { _orderNumber = orderNumber_; }
void OrderDetails::setClientCode(const std::string& clientCode_) { _clientCode = clientCode_; }
void OrderDetails::setAlgoCode(const std::string& algoCode_) { _algoCode = algoCode_; }
void OrderDetails::setContractDescription(const std::string& contractDescription_) { _contractDescription = contractDescription_; }

//--------------------------------------------------------------------------------------
StockPacket::StockPacket() : _lastRequest(OrderRequest_NONE), _currentStatus(OrderStatus_NONE), _previousStatus(OrderStatus_NONE) {}
void StockPacket::setLastRequest(OrderRequest lastRequest_) { _lastRequest = lastRequest_; }
void StockPacket::setCurrentStatus(OrderStatus currentStatus_) { _currentStatus = currentStatus_; }
void StockPacket::setPreviousStatus(OrderStatus previousStatus_) { _previousStatus = previousStatus_; }

OrderRequest StockPacket::getLastRequest() const { return _lastRequest; }
OrderStatus  StockPacket::getCurrentStatus() const { return _currentStatus; }
OrderStatus  StockPacket::getPreviousStatus() const { return _previousStatus; }

bool StockPacket::execute(int price_, int quantity_, OrderRequest request_) {
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

void StockPacket::executionReport(OrderStatus orderStatus_) {
    setPreviousStatus(getCurrentStatus());
    setCurrentStatus(orderStatus_);
    getStrategyPtr()->orderEventManager(getUniqueClassIdentity());
    getStrategyPtr()->updateArthur(shared_from_this());
}

}// namespace Lancelot::API