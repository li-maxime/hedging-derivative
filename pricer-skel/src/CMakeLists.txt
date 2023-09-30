project(DateConverter)

set(HEADER_FILES
        AsianOption.hpp
        BasketOption.hpp
        BlackScholesModel.hpp
        HedgingResults.hpp
        json_helper.hpp
        MonteCarlo.hpp
        Option.hpp
        PerformanceOption.hpp
        PriceParameters.hpp
        PriceParametersHedge.hpp
        PricingResults.hpp
        Hedger.hpp
)

set(SOURCE_FILES
        AsianOption.cpp
        BasketOption.cpp
        BlackScholesModel.cpp
        HedgingResults.cpp
        json_helper.cpp
        MonteCarlo.cpp
        PerformanceOption.cpp
        PriceParameters.cpp
        PriceParametersHedge.cpp
        PricingResults.cpp
        Hedger.cpp
        Portfolio.cpp
        Portfolio.hpp
)

add_library(Library SHARED ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(Library nlohmann_json::nlohmann_json)
