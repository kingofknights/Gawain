//
// Created by kingofknights on 12/9/23.
//

#ifndef GAWAIN_INCLUDE_GAWAIN_API_STREAMDATA_STREAMHELPER_HPP_
#define GAWAIN_INCLUDE_GAWAIN_API_STREAMDATA_STREAMHELPER_HPP_

#include <memory>
#include <thread>
#include <vector>

namespace Lancelot {
using ThreadPointerT = std::unique_ptr<std::jthread>;
using ThreadGroupT   = std::vector<ThreadPointerT>;

namespace API {
    class StreamHelper {
      public:
        virtual ~StreamHelper() = default;

        virtual void initialization(ThreadGroupT& threadGroup_) = 0;

        static void updateTick(uint32_t token_);
    };
}// namespace API
}// namespace Lancelot
#endif// GAWAIN_INCLUDE_GAWAIN_API_STREAMDATA_STREAMHELPER_HPP_
