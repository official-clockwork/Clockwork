#include "speedtest.hpp"
#include "position.hpp"
#include "repetition_info.hpp"
#include "search.hpp"
#include "tm.hpp"
#include "util/types.hpp"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace Clockwork {
namespace Speedtest {

const std::vector<std::string> SPEEDTEST_FENS = {
  "rnbqr1k1/pp3ppp/2pb1p2/8/3P4/2PB4/PPQ2PPP/R1B1K1NR w KQ - 5 9",
  "rnbqr1k1/pp3ppp/2pb1p2/8/3P4/2PB4/PPQ1NPPP/R1B1K2R b KQ - 6 9",
  "rnbqr1k1/pp3p1p/2pb1pp1/8/3P4/2PB4/PPQ1NPPP/R1B1K2R w KQ - 0 10",
  "rnbqr1k1/pp3p1p/2pb1pp1/8/3P3P/2PB4/PPQ1NPP1/R1B1K2R b KQ - 0 10",
  "rn1qr1k1/pp3p1p/2pbbpp1/8/3P3P/2PB4/PPQ1NPP1/R1B1K2R w KQ - 1 11",
  "rn1qr1k1/pp3p1p/2pbbpp1/7P/3P4/2PB4/PPQ1NPP1/R1B1K2R b KQ - 0 11",
  "rn1qr1k1/pp3p1p/2pbb1p1/5p1P/3P4/2PB4/PPQ1NPP1/R1B1K2R w KQ - 0 12",
  "rn1qr1k1/pp3p1p/2pbb1p1/5p1P/2PP4/3B4/PPQ1NPP1/R1B1K2R b KQ - 0 12",
  "r2qr1k1/pp1n1p1p/2pbb1p1/5p1P/2PP4/3B4/PPQ1NPP1/R1B1K2R w KQ - 1 13",
  "r2qr1k1/pp1n1p1p/2pbb1P1/5p2/2PP4/3B4/PPQ1NPP1/R1B1K2R b KQ - 0 13",
  "r2qr1k1/pp1n1p2/2pbb1p1/5p2/2PP4/3B4/PPQ1NPP1/R1B1K2R w KQ - 0 14",
  "r2qr1k1/pp1n1p2/2pbb1p1/5p2/2PP2P1/3B4/PPQ1NP2/R1B1K2R b KQ - 0 14",
  "r2qr1k1/pp1n1p2/2pbb1p1/8/2PP1pP1/3B4/PPQ1NP2/R1B1K2R w KQ - 0 15",
  "r2qr1k1/pp1n1p2/2pbb1B1/8/2PP1pP1/8/PPQ1NP2/R1B1K2R b KQ - 0 15",
  "r3r1k1/pp1n1p2/2pbb1B1/6q1/2PP1pP1/8/PPQ1NP2/R1B1K2R w KQ - 1 16",
  "r3r1k1/pp1n1p2/2pbb3/6q1/2PP1pP1/3B4/PPQ1NP2/R1B1K2R b KQ - 2 16",
  "r3r1k1/pp1n1p2/2pb4/6q1/2PP1pb1/3B4/PPQ1NP2/R1B1K2R w KQ - 0 17",
  "r3r1k1/pp1n1p2/2pb4/6q1/2PP1pb1/3B4/PPQBNP2/R3K2R b KQ - 1 17",
  "r5k1/pp1n1p2/2pb4/6q1/2PP1pb1/3B4/PPQBrP2/R3K2R w KQ - 0 18",
  "r5k1/pp1n1p2/2pb4/6q1/2PP1pb1/8/PPQBBP2/R3K2R b KQ - 0 18",
  "rnbq1rk1/1p2ppbp/6p1/p1Pp4/1PPNn3/4P3/P2N1PPP/R1BQKB1R w KQ - 0 9",
  "rnbq1rk1/1p2ppbp/6p1/p1Pp4/1PPNN3/4P3/P4PPP/R1BQKB1R b KQ - 0 9",
  "rnbq1rk1/1p2ppbp/6p1/p1P5/1PPNp3/4P3/P4PPP/R1BQKB1R w KQ - 0 10",
  "rnbq1rk1/1p2ppbp/6p1/pPP5/2PNp3/4P3/P4PPP/R1BQKB1R b KQ - 0 10",
  "rnb2rk1/1pq1ppbp/6p1/pPP5/2PNp3/4P3/P4PPP/R1BQKB1R w KQ - 1 11",
  "rnb2rk1/1pq1ppbp/6p1/pPP5/2PNp3/B3P3/P4PPP/R2QKB1R b KQ - 2 11",
  "rnbr2k1/1pq1ppbp/6p1/pPP5/2PNp3/B3P3/P4PPP/R2QKB1R w KQ - 3 12",
  "rnbr2k1/1pq1ppbp/6p1/pPP5/2PNp3/B3P3/P1Q2PPP/R3KB1R b KQ - 4 12",
  "r1br2k1/1pqnppbp/6p1/pPP5/2PNp3/B3P3/P1Q2PPP/R3KB1R w KQ - 5 13",
  "r1br2k1/1pqnppbp/1P4p1/p1P5/2PNp3/B3P3/P1Q2PPP/R3KB1R b KQ - 0 13",
  "r1br2k1/1p1nppbp/1P4p1/p1P1q3/2PNp3/B3P3/P1Q2PPP/R3KB1R w KQ - 1 14",
  "r1br2k1/1p1nppbp/1PP3p1/p3q3/2PNp3/B3P3/P1Q2PPP/R3KB1R b KQ - 0 14",
  "r1br2k1/3nppbp/1Pp3p1/p3q3/2PNp3/B3P3/P1Q2PPP/R3KB1R w KQ - 0 15",
  "r1br2k1/3nppbp/1Pp3p1/p3q3/2PNp3/B3P3/P1Q2PPP/2R1KB1R b K - 1 15",
  "r1br2k1/3nppbp/1Pp2qp1/p7/2PNp3/B3P3/P1Q2PPP/2R1KB1R w K - 2 16",
  "r1br2k1/3nppbp/1Pp2qp1/p1P5/3Np3/B3P3/P1Q2PPP/2R1KB1R b K - 0 16",
  "r1br2k1/3n1pbp/1Pp2qp1/p1P1p3/3Np3/B3P3/P1Q2PPP/2R1KB1R w K - 0 17",
  "r1br2k1/3n1pbp/1Pp2qp1/p1P1p3/4p3/BN2P3/P1Q2PPP/2R1KB1R b K - 1 17",
  "r1br1bk1/3n1p1p/1Pp2qp1/p1P1p3/4p3/BN2P3/P1Q2PPP/2R1KB1R w K - 2 18",
  "r1br1bk1/3n1p1p/1Pp2qp1/p1P1p3/4p3/BN2P3/PQ3PPP/2R1KB1R b K - 3 18",
  "r1bqk2r/pp3ppp/2n1pn2/3p4/2pP4/2P1PN2/P1P1BPPP/R1BQ1RK1 w kq - 1 9",
  "r1bqk2r/pp3ppp/2n1pn2/3pN3/2pP4/2P1P3/P1P1BPPP/R1BQ1RK1 b kq - 2 9",
  "r1b1k2r/pp3ppp/2n1pn2/q2pN3/2pP4/2P1P3/P1P1BPPP/R1BQ1RK1 w kq - 3 10",
  "r1b1k2r/pp3ppp/2N1pn2/q2p4/2pP4/2P1P3/P1P1BPPP/R1BQ1RK1 b kq - 0 10",
  "r1b1k2r/p4ppp/2p1pn2/q2p4/2pP4/2P1P3/P1P1BPPP/R1BQ1RK1 w kq - 0 11",
  "r1b1k2r/p4ppp/2p1pn2/q2p4/P1pP4/2P1P3/2P1BPPP/R1BQ1RK1 b kq - 0 11",
  "r1b2rk1/p4ppp/2p1pn2/q2p4/P1pP4/2P1P3/2P1BPPP/R1BQ1RK1 w - - 1 12",
  "r1b2rk1/p4ppp/2p1pn2/q2p4/P1pP4/B1P1P3/2P1BPPP/R2Q1RK1 b - - 2 12",
  "r1b1r1k1/p4ppp/2p1pn2/q2p4/P1pP4/B1P1P3/2P1BPPP/R2Q1RK1 w - - 3 13",
  "r1b1r1k1/p4ppp/2p1pn2/q2p4/P1pP4/B1P1P3/2P1BPPP/R3QRK1 b - - 4 13",
  "r1b1r1k1/p1q2ppp/2p1pn2/3p4/P1pP4/B1P1P3/2P1BPPP/R3QRK1 w - - 5 14",
  "r1b1r1k1/p1q2ppp/2p1pn2/3p4/P1pP1P2/B1P1P3/2P1B1PP/R3QRK1 b - - 0 14",
  "r1b1r1k1/2q2ppp/2p1pn2/p2p4/P1pP1P2/B1P1P3/2P1B1PP/R3QRK1 w - - 0 15",
  "r1b1r1k1/2q2ppp/2p1pn2/p2p4/P1pP1P2/B1P1PB2/2P3PP/R3QRK1 b - - 1 15",
  "r3r1k1/1bq2ppp/2p1pn2/p2p4/P1pP1P2/B1P1PB2/2P3PP/R3QRK1 w - - 2 16",
  "r3r1k1/1bq2ppp/2p1pn2/p2p4/P1pP1P2/B1P1PB2/2P3PP/1R2QRK1 b - - 3 16",
  "1r2r1k1/1bq2ppp/2p1pn2/p2p4/P1pP1P2/B1P1PB2/2P3PP/1R2QRK1 w - - 4 17",
  "1r2r1k1/1bq2ppp/2p1pn2/p2p4/P1pP1P2/B1P1PBQ1/2P3PP/1R3RK1 b - - 5 17",
  "1r2r1k1/1bq2ppp/4pn2/p1pp4/P1pP1P2/B1P1PBQ1/2P3PP/1R3RK1 w - - 0 18",
  "1r2r1k1/1bq2ppp/4pn2/p1Bp4/P1pP1P2/2P1PBQ1/2P3PP/1R3RK1 b - - 0 18",
  "r1bqkb1r/1pp2ppp/p1p1n3/4N3/8/8/PPPP1PPP/RNBQR1K1 w kq - 1 9",
  "r1bqkb1r/1pp2ppp/p1p1n3/4N3/8/7P/PPPP1PP1/RNBQR1K1 b kq - 0 9",
  "r1bqk2r/1pp2ppp/p1pbn3/4N3/8/7P/PPPP1PP1/RNBQR1K1 w kq - 1 10",
  "r1bqk2r/1pp2ppp/p1pbn3/4N3/8/3P3P/PPP2PP1/RNBQR1K1 b kq - 0 10",
  "r1bq1rk1/1pp2ppp/p1pbn3/4N3/8/3P3P/PPP2PP1/RNBQR1K1 w - - 1 11",
  "r1bq1rk1/1pp2ppp/p1pbn3/4N3/8/3P3P/PPPN1PP1/R1BQR1K1 b - - 2 11",
  "r1bq1rk1/1pp2ppp/p1p1n3/4b3/8/3P3P/PPPN1PP1/R1BQR1K1 w - - 0 12",
  "r1bq1rk1/1pp2ppp/p1p1n3/4R3/8/3P3P/PPPN1PP1/R1BQ2K1 b - - 0 12",
  "r1bq1rk1/2p2ppp/ppp1n3/4R3/8/3P3P/PPPN1PP1/R1BQ2K1 w - - 0 13",
  "r1bq1rk1/2p2ppp/ppp1n3/4R3/2N5/3P3P/PPP2PP1/R1BQ2K1 b - - 1 13",
  "r1b2rk1/2p2ppp/ppp1nq2/4R3/2N5/3P3P/PPP2PP1/R1BQ2K1 w - - 2 14",
  "r1b2rk1/2p2ppp/ppp1nq2/4R2Q/2N5/3P3P/PPP2PP1/R1B3K1 b - - 3 14",
  "r1b2rk1/2p2ppp/pp2nq2/2p1R2Q/2N5/3P3P/PPP2PP1/R1B3K1 w - - 0 15",
  "r1b2rk1/2p2ppp/pp2nq2/2p1R2Q/P1N5/3P3P/1PP2PP1/R1B3K1 b - - 0 15",
  "r1b2rk1/2p2ppp/pp2n1q1/2p1R2Q/P1N5/3P3P/1PP2PP1/R1B3K1 w - - 1 16",
  "r1b2rk1/2p2ppp/pp2n1Q1/2p1R3/P1N5/3P3P/1PP2PP1/R1B3K1 b - - 0 16",
  "r1b2rk1/2p2pp1/pp2n1p1/2p1R3/P1N5/3P3P/1PP2PP1/R1B3K1 w - - 0 17",
  "r1b2rk1/2p2pp1/pp2n1p1/2p1R3/P1N5/1P1P3P/2P2PP1/R1B3K1 b - - 0 17",
  "r1b2rk1/2p2pp1/pp2n3/2p1R1p1/P1N5/1P1P3P/2P2PP1/R1B3K1 w - - 0 18",
  "r1b2rk1/2p2pp1/pp2n3/2p3p1/P1N5/1P1P3P/2P2PP1/R1B1R1K1 b - - 1 18",
  "r3kb1r/pp1bpp1p/2np1p2/q7/3NP3/2N5/PPP1BPPP/R2QK2R w KQkq - 0 9",
  "r3kb1r/pp1bpp1p/2np1p2/q7/3NP3/2N5/PPP1BPPP/R2Q1RK1 b kq - 1 9",
  "r3kb1r/pp1bpp1p/3p1p2/q7/3nP3/2N5/PPP1BPPP/R2Q1RK1 w kq - 0 10",
  "r3kb1r/pp1bpp1p/3p1p2/q7/3QP3/2N5/PPP1BPPP/R4RK1 b kq - 0 10",
  "2r1kb1r/pp1bpp1p/3p1p2/q7/3QP3/2N5/PPP1BPPP/R4RK1 w k - 1 11",
  "2r1kb1r/pp1bpp1p/3p1p2/q2N4/3QP3/8/PPP1BPPP/R4RK1 b k - 2 11",
  "2r1kb1r/pp1bpp1p/3p1p2/2qN4/3QP3/8/PPP1BPPP/R4RK1 w k - 3 12",
  "2r1kb1r/pp1bpp1p/3p1p2/2qN4/4P3/8/PPPQBPPP/R4RK1 b k - 4 12",
  "2r1kb1r/pp1bpp1p/3p1p2/3N4/4P3/8/PPqQBPPP/R4RK1 w k - 0 13",
  "2r1kb1r/pp1bpp1p/3p1p2/3N4/4P3/4Q3/PPq1BPPP/R4RK1 b k - 1 13",
  "2r1k2r/pp1bpp1p/3p1p1b/3N4/4P3/4Q3/PPq1BPPP/R4RK1 w k - 2 14",
  "2r1k2r/pp1bpp1p/3p1p1Q/3N4/4P3/8/PPq1BPPP/R4RK1 b k - 0 14",
  "2r1k2r/pp1bpp1p/3p1p1Q/3N4/4P3/8/PP2qPPP/R4RK1 w k - 0 15",
  "2r1k2r/pp1bpp1p/3p1p1Q/3N4/4P3/8/PP2qPPP/R3R1K1 b k - 1 15",
  "2r1k2r/pp1bpp1p/3p1p1Q/3N4/4P1q1/8/PP3PPP/R3R1K1 w k - 2 16",
  "2r1k2r/pp1bpp1p/3p1p2/3N4/4P1q1/4Q3/PP3PPP/R3R1K1 b k - 3 16",
  "2r1k2r/pp2pp1p/2bp1p2/3N4/4P1q1/4Q3/PP3PPP/R3R1K1 w k - 4 17",
  "2r1k2r/Qp2pp1p/2bp1p2/3N4/4P1q1/8/PP3PPP/R3R1K1 b k - 0 17",
  "2r1k1r1/Qp2pp1p/2bp1p2/3N4/4P1q1/8/PP3PPP/R3R1K1 w - - 1 18",
  "2r1k1r1/Qp2pp1p/2bp1p2/3N4/4P1q1/6P1/PP3P1P/R3R1K1 b - - 0 18",
  "r1bk1b1r/ppp2ppp/2p5/4Pn2/8/5N2/PPP2PPP/RNB2RK1 w - - 0 9",
  "r1bk1b1r/ppp2ppp/2p5/4Pn2/8/5N2/PPP2PPP/RNBR2K1 b - - 1 9",
  "r1b1kb1r/ppp2ppp/2p5/4Pn2/8/5N2/PPP2PPP/RNBR2K1 w - - 2 10",
  "r1b1kb1r/ppp2ppp/2p5/4Pn2/8/2N2N2/PPP2PPP/R1BR2K1 b - - 3 10",
  "r1b1k2r/ppp1bppp/2p5/4Pn2/8/2N2N2/PPP2PPP/R1BR2K1 w - - 4 11",
  "r1b1k2r/ppp1bppp/2p5/4Pn2/8/2N2N1P/PPP2PP1/R1BR2K1 b - - 0 11",
  "r1b1k2r/ppp1bpp1/2p5/4Pn1p/8/2N2N1P/PPP2PP1/R1BR2K1 w - - 0 12",
  "r1b1k2r/ppp1bpp1/2p5/4Pn1p/4N3/5N1P/PPP2PP1/R1BR2K1 b - - 1 12",
  "r1b1k2r/ppp1bpp1/2p5/4Pn2/4N2p/5N1P/PPP2PP1/R1BR2K1 w - - 0 13",
  "r1b1k2r/ppp1bpp1/2p5/4Pn2/4NB1p/5N1P/PPP2PP1/R2R2K1 b - - 1 13",
  "r3k2r/ppp1bpp1/2p1b3/4Pn2/4NB1p/5N1P/PPP2PP1/R2R2K1 w - - 2 14",
  "r3k2r/ppp1bpp1/2p1b3/4Pn2/4NB1p/1P3N1P/P1P2PP1/R2R2K1 b - - 0 14",
  "3rk2r/ppp1bpp1/2p1b3/4Pn2/4NB1p/1P3N1P/P1P2PP1/R2R2K1 w - - 1 15",
  "3rk2r/ppp1bpp1/2p1b3/4PnN1/4NB1p/1P5P/P1P2PP1/R2R2K1 b - - 2 15",
  "4k2r/ppp1bpp1/2p1b3/4PnN1/4NB1p/1P5P/P1P2PP1/R2r2K1 w - - 0 16",
  "4k2r/ppp1bpp1/2p1b3/4PnN1/4NB1p/1P5P/P1P2PP1/3R2K1 b - - 0 16",
  "2b1k2r/ppp1bpp1/2p5/4PnN1/4NB1p/1P5P/P1P2PP1/3R2K1 w - - 1 17",
  "2b1k2r/ppp1bpp1/2p5/4PnN1/2P1NB1p/1P5P/P4PP1/3R2K1 b - - 0 17",
  "2b1k3/ppp1bpp1/2p5/4PnNr/2P1NB1p/1P5P/P4PP1/3R2K1 w - - 1 18",
  "2b1k3/ppp1bpp1/2p5/4Pn1r/2P1NB1p/1P3N1P/P4PP1/3R2K1 b - - 2 18",
  "r1bq1rk1/pppnbppp/4pn2/2Pp4/3P4/5NP1/PP1BPPBP/RN1Q1RK1 w - - 1 9",
  "r1bq1rk1/pppnbppp/4pn2/2Pp4/3P4/5NP1/PPQBPPBP/RN3RK1 b - - 2 9",
  "r1bq1rk1/pppnbppp/4p3/2Pp4/3Pn3/5NP1/PPQBPPBP/RN3RK1 w - - 3 10",
  "r1bq1rk1/pppnbppp/4p3/2Pp4/3Pn3/4BNP1/PPQ1PPBP/RN3RK1 b - - 4 10",
  "r1bq1rk1/pp1nbppp/2p1p3/2Pp4/3Pn3/4BNP1/PPQ1PPBP/RN3RK1 w - - 0 11",
  "r1bq1rk1/pp1nbppp/2p1p3/2Pp4/3Pn3/4BNP1/PPQNPPBP/R4RK1 b - - 1 11",
  "r1bq1rk1/pp1nbppp/2p1pn2/2Pp4/3P4/4BNP1/PPQNPPBP/R4RK1 w - - 2 12",
  "r1bq1rk1/pp1nbppp/2p1pn2/2Pp4/3P4/4BNP1/PPQNPPBP/R1R3K1 b - - 3 12",
  "r1bq1rk1/pp1nbppp/2p1p3/2Pp4/3P2n1/4BNP1/PPQNPPBP/R1R3K1 w - - 4 13",
  "r1bq1rk1/pp1nbppp/2p1p3/2Pp4/3P1Bn1/5NP1/PPQNPPBP/R1R3K1 b - - 5 13",
  "r1bq1rk1/pp1nbppp/2p5/2Ppp3/3P1Bn1/5NP1/PPQNPPBP/R1R3K1 w - - 0 14",
  "r1bq1rk1/pp1nbppp/2p5/2PpP3/5Bn1/5NP1/PPQNPPBP/R1R3K1 b - - 0 14",
  "r1bq1rk1/pp1nbp1p/2p5/2PpP1p1/5Bn1/5NP1/PPQNPPBP/R1R3K1 w - - 0 15",
  "r1bq1rk1/pp1nbp1p/2p5/2PpP1N1/5Bn1/6P1/PPQNPPBP/R1R3K1 b - - 0 15",
  "r1bq1rk1/pp1n1p1p/2p5/2PpP1b1/5Bn1/6P1/PPQNPPBP/R1R3K1 w - - 0 16",
  "r1bq1rk1/pp1n1p1p/2p5/2PpPQb1/5Bn1/6P1/PP1NPPBP/R1R3K1 b - - 1 16",
  "r1bq1rk1/pp1n1p1p/2p5/2PpPQ2/5bn1/6P1/PP1NPPBP/R1R3K1 w - - 0 17",
  "r1bq1rk1/pp1n1p1p/2p5/2PpP3/5bQ1/6P1/PP1NPPBP/R1R3K1 b - - 0 17",
  "r1bq1rk1/pp1n1p1p/2p5/2PpP1b1/6Q1/6P1/PP1NPPBP/R1R3K1 w - - 1 18",
  "r1bq1rk1/pp1n1p1p/2p5/2PpP1b1/6QP/6P1/PP1NPPB1/R1R3K1 b - - 0 18",
  "r3kbnr/1bqp1ppp/p1n1p3/1p6/3QP3/P1N2N2/1PP1BPPP/R1B2RK1 w kq - 1 9",
  "r3kbnr/1bqp1ppp/p1n1p3/1p6/4P3/P1N1QN2/1PP1BPPP/R1B2RK1 b kq - 2 9",
  "r3k1nr/1bqp1ppp/p1nbp3/1p6/4P3/P1N1QN2/1PP1BPPP/R1B2RK1 w kq - 3 10",
  "r3k1nr/1bqp1ppp/p1nbp3/1p4Q1/4P3/P1N2N2/1PP1BPPP/R1B2RK1 b kq - 4 10",
  "r3k1nr/1bqp1ppp/p2bp3/1p2n1Q1/4P3/P1N2N2/1PP1BPPP/R1B2RK1 w kq - 5 11",
  "r3k1nr/1bqp1pQp/p2bp3/1p2n3/4P3/P1N2N2/1PP1BPPP/R1B2RK1 b kq - 0 11",
  "r3k1nr/1bqp1pQp/p2bp3/1p6/4P3/P1N2n2/1PP1BPPP/R1B2RK1 w kq - 0 12",
  "r3k1nr/1bqp1pQp/p2bp3/1p6/4P3/P1N2B2/1PP2PPP/R1B2RK1 b kq - 0 12",
  "r3k1nr/1bqp1pQp/p3p3/1p6/4P3/P1N2B2/1PP2PPb/R1B2RK1 w kq - 0 13",
  "r3k1nr/1bqp1pQp/p3p3/1p6/4P3/P1N2B2/1PP2PPb/R1B2R1K b kq - 1 13",
  "r3k1nr/1bqp1pQp/p3p3/1p2b3/4P3/P1N2B2/1PP2PP1/R1B2R1K w kq - 2 14",
  "r3k1nr/1bqp1p1p/p3p3/1p2b1Q1/4P3/P1N2B2/1PP2PP1/R1B2R1K b kq - 3 14",
  "r3k1nr/1bqp1p2/p3p2p/1p2b1Q1/4P3/P1N2B2/1PP2PP1/R1B2R1K w kq - 0 15",
  "r3k1nr/1bqp1p2/p3p2p/1p2b3/4P2Q/P1N2B2/1PP2PP1/R1B2R1K b kq - 1 15",
  "r2qk1nr/1b1p1p2/p3p2p/1p2b3/4P2Q/P1N2B2/1PP2PP1/R1B2R1K w kq - 2 16",
  "r2qk1nr/1b1p1p2/p3p2p/1p2b3/4P3/P1N2B1Q/1PP2PP1/R1B2R1K b kq - 3 16",
  "r2qk2r/1b1p1p2/p3pn1p/1p2b3/4P3/P1N2B1Q/1PP2PP1/R1B2R1K w kq - 4 17",
  "r2qk2r/1b1p1p2/p3pn1p/1p2b3/4P3/P1N2B1Q/1PPB1PP1/R4R1K b kq - 5 17",
  "r2qk1r1/1b1p1p2/p3pn1p/1p2b3/4P3/P1N2B1Q/1PPB1PP1/R4R1K w q - 6 18",
  "r2qk1r1/1b1p1p2/p3pn1p/1p2b3/4P3/P1N2B1Q/1PPB1PP1/R3R2K b q - 7 18",
  "r1bqk2r/1p1nbppp/p1n1p3/2ppP3/3P1P2/2N1BN2/PPPQ2PP/R3KB1R w KQkq - 0 9",
  "r1bqk2r/1p1nbppp/p1n1p3/2PpP3/5P2/2N1BN2/PPPQ2PP/R3KB1R b KQkq - 0 9",
  "r1bqk2r/1p1n1ppp/p1n1p3/2bpP3/5P2/2N1BN2/PPPQ2PP/R3KB1R w KQkq - 0 10",
  "r1bqk2r/1p1n1ppp/p1n1p3/2bpP3/5P2/2N1BN2/PPPQ2PP/2KR1B1R b kq - 1 10",
  "r1b1k2r/1p1n1ppp/pqn1p3/2bpP3/5P2/2N1BN2/PPPQ2PP/2KR1B1R w kq - 2 11",
  "r1b1k2r/1p1n1ppp/pqn1p3/2bpP3/3N1P2/2N1B3/PPPQ2PP/2KR1B1R b kq - 3 11",
  "r1b1k2r/1p1n1ppp/pqn1p3/3pP3/3b1P2/2N1B3/PPPQ2PP/2KR1B1R w kq - 0 12",
  "r1b1k2r/1p1n1ppp/pqn1p3/3pP3/3B1P2/2N5/PPPQ2PP/2KR1B1R b kq - 0 12",
  "r1b1k2r/1p1n1ppp/p1n1p3/3pP3/3q1P2/2N5/PPPQ2PP/2KR1B1R w kq - 0 13",
  "r1b1k2r/1p1n1ppp/p1n1p3/3pP3/3Q1P2/2N5/PPP3PP/2KR1B1R b kq - 0 13",
  "r1b1k2r/1p1n1ppp/p3p3/3pP3/3n1P2/2N5/PPP3PP/2KR1B1R w kq - 0 14",
  "r1b1k2r/1p1n1ppp/p3p3/3pP3/3R1P2/2N5/PPP3PP/2K2B1R b kq - 0 14",
  "rnb1k2r/1p3ppp/p3p3/3pP3/3R1P2/2N5/PPP3PP/2K2B1R w kq - 1 15",
  "rnb1k2r/1p3ppp/p3p3/3pP3/R4P2/2N5/PPP3PP/2K2B1R b kq - 2 15",
  "rn2k2r/1p1b1ppp/p3p3/3pP3/R4P2/2N5/PPP3PP/2K2B1R w kq - 3 16",
  "rn2k2r/1p1b1ppp/p3p3/3pP3/5P2/R1N5/PPP3PP/2K2B1R b kq - 4 16",
  "rn5r/1p1bkppp/p3p3/3pP3/5P2/R1N5/PPP3PP/2K2B1R w - - 5 17",
  "rn5r/1p1bkppp/p3p3/3pP3/5P2/R1NB4/PPP3PP/2K4R b - - 6 17",
  "rn5r/1p1bkpp1/p3p3/3pP2p/5P2/R1NB4/PPP3PP/2K4R w - - 0 18",
  "rn5r/1p1bkpp1/p3p3/3pP2p/5P2/R1NB4/PPP3PP/2K1R3 b - - 1 18",
  "rn1qkb1r/1bp2ppp/p3pn2/6B1/PppPN3/4PN2/1P3PPP/R2QKB1R w KQkq - 2 9",
  "rn1qkb1r/1bp2ppp/p3pN2/6B1/PppP4/4PN2/1P3PPP/R2QKB1R b KQkq - 0 9",
  "rn1qkb1r/1bp2p1p/p3pp2/6B1/PppP4/4PN2/1P3PPP/R2QKB1R w KQkq - 0 10",
  "rn1qkb1r/1bp2p1p/p3pp2/8/PppP3B/4PN2/1P3PPP/R2QKB1R b KQkq - 1 10",
  "rn1qkb1r/2p2p1p/p3pp2/3b4/PppP3B/4PN2/1P3PPP/R2QKB1R w KQkq - 2 11",
  "rn1qkb1r/2p2p1p/p3pp2/3b4/PppP3B/4PN2/1P3PPP/2RQKB1R b Kkq - 3 11",
  "rn1qkb1r/2p2p1p/p3pp2/3b4/Pp1P3B/2p1PN2/1P3PPP/2RQKB1R w Kkq - 0 12",
  "rn1qkb1r/2p2p1p/p3pp2/3b4/Pp1P3B/2P1PN2/5PPP/2RQKB1R b Kkq - 0 12",
  "rn1qkb1r/2p2p1p/p3pp2/3b4/P2P3B/2p1PN2/5PPP/2RQKB1R w Kkq - 0 13",
  "rn1qkb1r/2p2p1p/p3pp2/3b4/P2P3B/2pBPN2/5PPP/2RQK2R b Kkq - 1 13",
  "r2qkb1r/2pn1p1p/p3pp2/3b4/P2P3B/2pBPN2/5PPP/2RQK2R w Kkq - 2 14",
  "r2qkb1r/2pn1p1p/p3pp2/3b4/P2P3B/2pBPN2/5PPP/2RQ1RK1 b kq - 3 14",
  "r2qk2r/2pn1p1p/p3pp2/3b4/Pb1P3B/2pBPN2/5PPP/2RQ1RK1 w kq - 4 15",
  "2qk2r/2pn1p1p/p3pp2/3b4/Pb1PP2B/2pB1N2/5PPP/2RQ1RK1 b kq - 0 15",
  "r2qk2r/1bpn1p1p/p3pp2/8/Pb1PP2B/2pB1N2/5PPP/2RQ1RK1 w kq - 1 16",
  "r2qk2r/1bpn1p1p/p3pp2/8/Pb1PP2B/1QpB1N2/5PPP/2R2RK1 b kq - 2 16",
  "r2qk2r/1bpn1p1p/4pp2/p7/Pb1PP2B/1QpB1N2/5PPP/2R2RK1 w kq - 0 17",
  "r2qk2r/1bpn1p1p/4pp2/p7/Pb1PP2B/1QRB1N2/5PPP/5RK1 b kq - 0 17",
  "r2qk2r/1bpn1p1p/4pp2/p7/P2PP2B/1QbB1N2/5PPP/5RK1 w kq - 0 18",
  "r2qk2r/1Qpn1p1p/4pp2/p7/P2PP2B/2bB1N2/5PPP/5RK1 b kq - 0 18",
  "r1bq1rk1/bppp1pp1/p1n2n1p/4p1B1/B2PP3/2P2N2/PP3PPP/RN1Q1RK1 w - - 0 9",
  "r1bq1rk1/bppp1pp1/p1n2n1p/4p3/B2PP2B/2P2N2/PP3PPP/RN1Q1RK1 b - - 1 9",
  "r1bq1rk1/bppp1p2/p1n2n1p/4p1p1/B2PP2B/2P2N2/PP3PPP/RN1Q1RK1 w - - 0 10",
  "r1bq1rk1/bppp1p2/p1n2n1p/4p1N1/B2PP2B/2P5/PP3PPP/RN1Q1RK1 b - - 0 10",
  "r1bq1rk1/bppp1p2/p1n2n2/4p1p1/B2PP2B/2P5/PP3PPP/RN1Q1RK1 w - - 0 11",
  "r1bq1rk1/bppp1p2/p1n2n2/4p1B1/B2PP3/2P5/PP3PPP/RN1Q1RK1 b - - 0 11",
  "r1bqr1k1/bppp1p2/p1n2n2/4p1B1/B2PP3/2P5/PP3PPP/RN1Q1RK1 w - - 1 12",
  "r1bqr1k1/bppp1p2/p1n2n2/4p1B1/B2PPP2/2P5/PP4PP/RN1Q1RK1 b - - 0 12",
  "r1bqr1k1/bppp1p2/p4n2/4p1B1/B2nPP2/2P5/PP4PP/RN1Q1RK1 w - - 0 13",
  "r1bqr1k1/bppp1p2/p4n2/4p1B1/B2PPP2/8/PP4PP/RN1Q1RK1 b - - 0 13",
  "r1bqr1k1/1ppp1p2/p4n2/4p1B1/B2bPP2/8/PP4PP/RN1Q1RK1 w - - 0 14",
  "r1bqr1k1/1ppp1p2/p4n2/4p1B1/B2bPP2/8/PP4PP/RN1Q1R1K b - - 1 14",
  "r1bqr1k1/1ppp1p2/p4n2/4p1B1/B3PP2/8/Pb4PP/RN1Q1R1K w - - 0 15",
  "r1bqr1k1/1ppp1p2/p4n2/4P1B1/B3P3/8/Pb4PP/RN1Q1R1K b - - 0 15",
  "r1bqr1k1/1ppp1p2/p4n2/4b1B1/B3P3/8/P5PP/RN1Q1R1K w - - 0 16",
  "r1bqr1k1/1ppp1p2/p4n2/4b1B1/B3P3/8/P2N2PP/R2Q1R1K b - - 1 16",
  "r1b1r1k1/1pppqp2/p4n2/4b1B1/B3P3/8/P2N2PP/R2Q1R1K w - - 2 17",
  "r1b1r1k1/1pppqp2/p4n2/4b1B1/B3P3/8/P2N2PP/2RQ1R1K b - - 3 17",
  "r1b1r1k1/2ppqp2/p4n2/1p2b1B1/B3P3/8/P2N2PP/2RQ1R1K w - - 0 18",
  "r1b1r1k1/2ppqp2/p4n2/1p2b1B1/B3P3/5N2/P5PP/2RQ1R1K b - - 1 18",
  "rn1qr1k1/1pp1ppbp/3p1np1/p4b2/2PP4/2N1PN1P/PP2BPP1/R1BQ1RK1 w - - 0 9",
  "rn1qr1k1/1pp1ppbp/3p1np1/p4b2/2PP2P1/2N1PN1P/PP2BP2/R1BQ1RK1 b - - 0 9",
  "rn1qr1k1/1ppbppbp/3p1np1/p7/2PP2P1/2N1PN1P/PP2BP2/R1BQ1RK1 w - - 1 10",
  "rn1qr1k1/1ppbppbp/3p1np1/p7/2PPP1P1/2N2N1P/PP2BP2/R1BQ1RK1 b - - 0 10",
  "rn1qr1k1/1ppb1pbp/3p1np1/p3p3/2PPP1P1/2N2N1P/PP2BP2/R1BQ1RK1 w - - 0 11",
  "rn1qr1k1/1ppb1pbp/3p1np1/p2Pp3/2P1P1P1/2N2N1P/PP2BP2/R1BQ1RK1 b - - 0 11",
  "r2qr1k1/1ppb1pbp/n2p1np1/p2Pp3/2P1P1P1/2N2N1P/PP2BP2/R1BQ1RK1 w - - 1 12",
  "r2qr1k1/1ppb1pbp/n2p1np1/p2Pp3/2P1P1P1/2N1BN1P/PP2BP2/R2Q1RK1 b - - 2 12",
  "r2qr1k1/1ppb1pbp/3p1np1/p1nPp3/2P1P1P1/2N1BN1P/PP2BP2/R2Q1RK1 w - - 3 13",
  "r2qr1k1/1ppb1pbp/3p1np1/p1nPp3/2P1P1P1/2N1BN1P/PPQ1BP2/R4RK1 b - - 4 13",
  "r2qr1k1/1ppb1pb1/3p1npp/p1nPp3/2P1P1P1/2N1BN1P/PPQ1BP2/R4RK1 w - - 0 14",
  "r2qr1k1/1ppb1pb1/3p1npp/p1nPp3/2P1P1P1/2N1BN1P/PPQ1BPK1/R4R2 b - - 1 14",
  "r2qr1k1/1p1b1pb1/2pp1npp/p1nPp3/2P1P1P1/2N1BN1P/PPQ1BPK1/R4R2 w - - 0 15",
  "r2qr1k1/1p1b1pb1/2pp1npp/p1nPp3/2P1P1P1/2N1B2P/PPQNBPK1/R4R2 b - - 1 15",
  "r2qr1k1/1p1b1pb1/2pp1np1/p1nPp2p/2P1P1P1/2N1B2P/PPQNBPK1/R4R2 w - - 0 16",
  "r2qr1k1/1p1b1pb1/2pp1np1/p1nPp2p/2P1P1P1/2N1B2P/PP1NBPK1/R2Q1R2 b - - 1 16",
  "r2qr1k1/1p1b1pb1/2pp1np1/2nPp2p/p1P1P1P1/2N1B2P/PP1NBPK1/R2Q1R2 w - - 0 17",
  "r2qr1k1/1p1b1pb1/2pp1np1/2nPp2p/p1P1P1P1/2N1B2P/PP1NBPK1/1R1Q1R2 b - - 1 17",
  "r2qr1k1/1p1b1pb1/3p1np1/2npp2p/p1P1P1P1/2N1B2P/PP1NBPK1/1R1Q1R2 w - - 0 18",
  "r2qr1k1/1p1b1pb1/3p1np1/2nPp2p/p3P1P1/2N1B2P/PP1NBPK1/1R1Q1R2 b - - 0 18",
  "r2qk2r/ppp1bppp/2np1n2/1B3b2/4P3/2N5/PPP2PPP/R1BQ1RK1 w kq - 0 9",
  "r2qk2r/ppp1bppp/2np1n2/1B3P2/8/2N5/PPP2PPP/R1BQ1RK1 b kq - 0 9",
  "r2q1rk1/ppp1bppp/2np1n2/1B3P2/8/2N5/PPP2PPP/R1BQ1RK1 w - - 1 10",
  "r2q1rk1/ppp1bppp/2np1n2/1B1N1P2/8/8/PPP2PPP/R1BQ1RK1 b - - 2 10",
  "r2q1rk1/ppp1bpp1/2np1n1p/1B1N1P2/8/8/PPP2PPP/R1BQ1RK1 w - - 0 11",
  "r2q1rk1/ppp1bpp1/2np1n1p/3N1P2/B7/8/PPP2PPP/R1BQ1RK1 b - - 1 11",
  "r2q1rk1/ppp1bpp1/2np3p/3n1P2/B7/8/PPP2PPP/R1BQ1RK1 w - - 0 12",
  "r2q1rk1/ppp1bpp1/2np3p/3Q1P2/B7/8/PPP2PPP/R1B2RK1 b - - 0 12",
  "1r1q1rk1/ppp1bpp1/2np3p/3Q1P2/B7/8/PPP2PPP/R1B2RK1 w - - 1 13",
  "1r1q1rk1/ppp1bpp1/2np3p/3Q1P2/B7/2P5/PP3PPP/R1B2RK1 b - - 0 13",
  "1r1q1rk1/ppp1bpp1/3p3p/3QnP2/B7/2P5/PP3PPP/R1B2RK1 w - - 1 14",
  "1r1q1rk1/ppp1bpp1/3p3p/3QnP2/8/2P5/PPB2PPP/R1B2RK1 b - - 2 14",
  "1r1q1rk1/pp2bpp1/2pp3p/3QnP2/8/2P5/PPB2PPP/R1B2RK1 w - - 0 15",
  "1r1q1rk1/pp2bpp1/2pp3p/4nP2/3Q4/2P5/PPB2PPP/R1B2RK1 b - - 1 15",
  "1r3rk1/pp2bpp1/1qpp3p/4nP2/3Q4/2P5/PPB2PPP/R1B2RK1 w - - 2 16",
  "1r3rk1/pp2bpp1/1qpp3p/4nP2/8/2P1Q3/PPB2PPP/R1B2RK1 b - - 3 16",
  "1r2r1k1/pp2bpp1/1qpp3p/4nP2/8/2P1Q3/PPB2PPP/R1B2RK1 w - - 4 17",
  "1r2r1k1/pp2bpp1/1qpp3p/4nP2/8/2P3Q1/PPB2PPP/R1B2RK1 b - - 5 17",
  "1r2r1k1/pp2bpp1/1qpp4/4nP1p/8/2P3Q1/PPB2PPP/R1B2RK1 w - - 0 18",
  "1r2r1k1/pp2bpp1/1qpp4/4nP1p/8/2P4Q/PPB2PPP/R1B2RK1 b - - 1 18",
  "4r3/pp1nkp2/2pb3R/8/2PP1pb1/8/PP1BBP2/R3K3 b Q - 0 22",
  "3kr3/pp1n1p2/2pb3R/8/2PP1pb1/8/PP1BBP2/R3K3 w Q - 1 23",
  "3kr3/pp1n1p2/2pb3R/8/2PP1pb1/8/PP1BBP2/2KR4 b - - 2 23",
  "3k4/pp1n1p2/2pb3R/8/2PP1pb1/8/PP1BrP2/2KR4 w - - 0 24",
  "3k4/pp1n1p2/2pR4/8/2PP1pb1/8/PP1BrP2/2KR4 b - - 0 24",
  "3k4/pp1n1p2/2pR4/8/2PP1pb1/8/PP1B1r2/2KR4 w - - 0 25",
  "3k4/pp1n1p2/2pR4/8/2PP1pb1/8/PP1B1r2/2K4R b - - 1 25",
  "3k4/pp1n1p2/2pR4/5b2/2PP1p2/8/PP1B1r2/2K4R w - - 2 26",
  "3k4/pp1n1p2/2p4R/5b2/2PP1p2/8/PP1B1r2/2K4R b - - 3 26",
  "3k4/pp1n1p2/2p4R/5b2/2PP4/5p2/PP1B1r2/2K4R w - - 0 27",
  "3k4/pp1n1p2/2p5/5b2/2PP4/5p2/PP1B1r1R/2K4R b - - 1 27",
  "3k4/pp1n1p2/2p5/5b2/2PP4/5p2/PP1B2rR/2K4R w - - 2 28",
  "3k4/pp1n1p2/2p5/5b2/2PP4/5p2/PP1B2R1/2K4R b - - 0 28",
  "3k4/pp1n1p2/2p5/5b2/2PP4/8/PP1B2p1/2K4R w - - 0 29",
  "3k3R/pp1n1p2/2p5/5b2/2PP4/8/PP1B2p1/2K5 b - - 1 29",
  "7R/pp1nkp2/2p5/5b2/2PP4/8/PP1B2p1/2K5 w - - 2 30",
  "7R/pp1nkp2/2p5/5b2/2PP4/4B3/PP4p1/2K5 b - - 3 30",
  "7R/1p1nkp2/2p5/p4b2/2PP4/4B3/PP4p1/2K5 w - - 0 31",
  "7R/1p1nkp2/2p5/p4b2/2PP4/8/PP4p1/2K3B1 b - - 1 31",
  "7R/1p1nkp2/2p5/p7/2PPb3/8/PP4p1/2K3B1 w - - 2 32",
  "4B3/2R5/7k/6p1/2p5/6P1/4KP1n/8 w - - 0 61",
  "4B3/8/7k/6p1/2R5/6P1/4KP1n/8 b - - 0 61",
  "4B3/6k1/8/6p1/2R5/6P1/4KP1n/8 w - - 1 62",
  "8/3B2k1/8/6p1/2R5/6P1/4KP1n/8 b - - 2 62",
  "8/3B1k2/8/6p1/2R5/6P1/4KP1n/8 w - - 3 63",
  "8/3B1k2/2R5/6p1/8/6P1/4KP1n/8 b - - 4 63",
  "8/3B2k1/2R5/6p1/8/6P1/4KP1n/8 w - - 5 64",
  "2B5/6k1/2R5/6p1/8/6P1/4KP1n/8 b - - 6 64",
  "2B5/6k1/2R5/8/6p1/6P1/4KP1n/8 w - - 0 65",
  "2B5/6k1/8/8/2R3p1/6P1/4KP1n/8 b - - 1 65",
  "2B5/8/5k2/8/2R3p1/6P1/4KP1n/8 w - - 2 66",
  "8/8/5k2/8/2R3B1/6P1/4KP1n/8 b - - 0 66",
  "8/8/5k2/8/2R3B1/6P1/4KP2/5n2 w - - 1 67",
  "8/8/5k2/8/2R3B1/6P1/5P2/5K2 b - - 0 67",
  "8/8/8/4k3/2R3B1/6P1/5P2/5K2 w - - 1 68",
  "8/8/2R5/4k3/6B1/6P1/5P2/5K2 b - - 2 68",
  "8/8/2R5/8/3k2B1/6P1/5P2/5K2 w - - 3 69",
  "8/8/2R5/8/3k2B1/6P1/4KP2/8 b - - 4 69",
  "8/8/2R5/8/4k1B1/6P1/4KP2/8 w - - 5 70",
  "8/8/2R5/8/4kPB1/6P1/4K3/8 b - - 0 70",
  "3k4/8/3PRB2/8/8/5p2/5Kp1/8 b - - 0 65",
  "8/3k4/3PRB2/8/8/5p2/5Kp1/8 w - - 1 66",
  "8/3k4/3P1B2/8/8/4Rp2/5Kp1/8 b - - 2 66",
  "8/8/3k1B2/8/8/4Rp2/5Kp1/8 w - - 0 67",
  "8/8/3k1B2/8/8/5R2/5Kp1/8 b - - 0 67",
  "8/8/3k1B2/8/8/5R2/8/6K1 b - - 0 68",
  "8/8/5B2/3k4/8/5R2/8/6K1 w - - 1 69",
  "8/8/8/3kB3/8/5R2/8/6K1 b - - 2 69",
  "8/8/8/4k3/8/5R2/8/6K1 w - - 0 70",
  "8/8/8/4k3/8/5R2/8/5K2 b - - 1 70",
  "8/8/3k4/8/8/5R2/8/5K2 w - - 2 71",
  "8/8/3k4/8/8/5R2/4K3/8 b - - 3 71",
  "8/8/8/3k4/8/5R2/4K3/8 w - - 4 72",
  "8/8/8/3k4/8/3K1R2/8/8 b - - 5 72",
  "8/8/3k4/8/8/3K1R2/8/8 w - - 6 73",
  "8/8/3k4/8/8/3KR3/8/8 b - - 7 73",
  "8/8/8/2k5/8/3KR3/8/8 w - - 8 74",
  "8/8/8/2k1R3/8/3K4/8/8 b - - 9 74",
  "8/8/3k4/4R3/8/3K4/8/8 w - - 10 75",
  "8/8/3k4/4R3/3K4/8/8/8 b - - 11 75",
};

void speedtest(Search::Searcher& searcher) {
    u64  total_nodes = 0;
    auto start_time  = time::Clock::now();

    searcher.wait();

    const size_t total_positions     = SPEEDTEST_FENS.size();
    u64          positions_processed = 0;

    for (const std::string& fen : SPEEDTEST_FENS) {
        auto pos = Position::parse(fen);
        if (!pos) {
            positions_processed++;
            continue;
        }

        int ply       = pos.value().get_ply();
        i64 move_time = 50000 / (ply + 15);

        Search::SearchSettings settings;
        settings.move_time = move_time;
        settings.silent    = true;

        RepetitionInfo dummy;
        searcher.set_position(pos.value(), dummy);
        searcher.launch_search(settings);
        searcher.wait();

        total_nodes += searcher.node_count();
        positions_processed++;

        if (positions_processed % 20 == 0 && positions_processed < total_positions) {
            searcher.tt.clear();
        }

        const int bar_width = 50;
        float     progress =
          static_cast<float>(positions_processed) / static_cast<float>(total_positions);
        int bar_pos = static_cast<int>(bar_width * progress);

        std::cout << "[";
        for (int i = 0; i < bar_width; ++i) {
            if (i < bar_pos) {
                std::cout << "#";
            } else {
                std::cout << " ";
            }
        }
        std::cout << "] " << positions_processed << "/" << total_positions << "\r";
        std::cout.flush();
    }

    auto   end_time    = time::Clock::now();
    auto   duration_ms = time::cast<time::Milliseconds>(end_time - start_time).count();
    f64    nps         = (duration_ms > 0)
                         ? (static_cast<f64>(total_nodes) * 1000.0 / static_cast<f64>(duration_ms))
                         : 0.0;

    std::cout << std::endl;
    std::cout << "Speedtest finished." << std::endl;
    std::cout << "Total positions: " << positions_processed << std::endl;
    std::cout << "Total nodes: " << total_nodes << std::endl;
    std::cout << "Total time: " << duration_ms << " ms" << std::endl;
    std::cout << "Nodes per second: " << static_cast<u64>(nps) << std::endl;
}

}  // namespace Speedtest
}  // namespace Clockwork
