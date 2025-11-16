#include <iomanip>
#include <iostream>

namespace Clockwork {

class IosFmtGuard {
public:
    explicit IosFmtGuard(std::ostream& stream) :
        m_stream(stream),
        m_state(nullptr) {
        m_state.copyfmt(m_stream);
    }

    ~IosFmtGuard() {
        m_stream.copyfmt(m_state);
    }

private:
    std::ostream& m_stream;
    std::ios      m_state;
};

}  // namespace Clockwork
