#include <factors/continuous/SemiparametricCPD.hpp>


namespace factors::continuous {


    const std::string& SemiparametricCPD::variable() const { 
        return std::visit([](auto& cpd) -> const std::string& {
                    return cpd.variable();
                }, m_cpd);
    }

    const std::vector<std::string>& SemiparametricCPD::evidence() const { 
        return std::visit([](auto& cpd) -> const std::vector<std::string>& {
                    return cpd.evidence();
                }, m_cpd);
    }

    bool SemiparametricCPD::fitted() const { 
        return std::visit([](auto& cpd) {
                    return cpd.fitted();
                }, m_cpd);
    }

    void SemiparametricCPD::fit(const DataFrame& df) {
        std::visit([&df](auto& cpd) {
            cpd.fit(df);
        }, m_cpd);
    }

    VectorXd SemiparametricCPD::logl(const DataFrame& df) const {
        return std::visit([&df](auto& cpd) -> VectorXd {
                            return cpd.logl(df);
                        }, m_cpd);
    }

    double SemiparametricCPD::slogl(const DataFrame& df) const {
        return std::visit([&df](auto& cpd) {
                    return cpd.slogl(df);
                }, m_cpd);
    }

    VectorXd SemiparametricCPD::cdf(const DataFrame& df) const {
        return std::visit([&df](auto& cpd) -> VectorXd {
                            return cpd.cdf(df);
                        }, m_cpd);
    }

    std::string SemiparametricCPD::ToString() const {
        return std::visit([](auto& cpd) {
                    return cpd.ToString();
                }, m_cpd);
    }

    Array_ptr SemiparametricCPD::sample(int n, const DataFrame& evidence_values, 
                                        unsigned int seed) const {
        
        return std::visit([n, &evidence_values, seed](auto& cpd) {
            return cpd.sample(n, evidence_values, seed);
        }, m_cpd);
    }

    py::tuple SemiparametricCPD::__getstate__() const {
        return py::make_tuple(m_cpd.index(),
                                std::visit([](auto& cpd) {
                                    return cpd.__getstate__();
                                }, m_cpd)
                             );  
    }

    SemiparametricCPD SemiparametricCPD::__setstate__(py::tuple& t) {
        if (t.size() != 2)
            throw std::runtime_error("Not valid SemiparametricCPD.");

        auto index = t[0].cast<size_t>();

        if (index == 0)
            return LinearGaussianCPD::__setstate__(t[1].cast<py::tuple>());
        else
            return CKDE::__setstate__(t[1].cast<py::tuple>());
    }
}