#pragma once
// Deliberately includes nothing: cluster.h pulls this header in mid-definition,
// so anything Fault touches (Cluster, Config) is still incomplete here. The
// bodies live in fault.cpp, where both classes are fully defined.
class Fault {
    public:
        // @TODO: We need a map of ErrorCodes in the brain so we dont bloat the report buffer
        static inline const int NO_FAULT = 0;
        static inline const int LOAD_TIMEOUT = -1;
        static inline const int REQ_UNPROCESSABLE = -2;
        static inline const int MISSING_OR_NO_CMD = -3;
        static inline const int MISSING_SLOT = -4;
        static inline const int BUSY =-5;
        static void clean();
        static void raise(int code);

};
