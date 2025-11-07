#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(293, 314);
inline const PParam KNIGHT_MAT = S(1136, 911);
inline const PParam BISHOP_MAT = S(1229, 949);
inline const PParam ROOK_MAT   = S(1712, 1681);
inline const PParam QUEEN_MAT  = S(3665, 2887);
inline const PParam TEMPO_VAL  = S(59, 15);

inline const PParam BISHOP_PAIR_VAL    = S(80, 177);
inline const PParam ROOK_OPEN_VAL      = S(104, -28);
inline const PParam ROOK_SEMIOPEN_VAL  = S(39, 13);

inline const PParam DOUBLED_PAWN_VAL = S(-37, -78);

inline const PParam POTENTIAL_CHECKER_VAL = S(-74, -2);
inline const PParam OUTPOST_KNIGHT_VAL    = S(7, 51);
inline const PParam OUTPOST_BISHOP_VAL    = S(43, 44);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(54, 18);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(56, -14);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(34, 33);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(55, -52);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(20, 19), S(63, 31), S(74, 70), S(191, 140), S(561, 241), S(931, 1149),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(64, 43), S(61, 31), S(67, 57), S(147, 120), S(689, -90),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-70, -103), S(-60, -85), S(-34, -9), S(19, 70), S(42, 211), S(306, 312),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(50, -44), S(36, -5), S(21, 27), S(24, 75), S(93, 151), S(144, 295),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(15, -45), S(3, 2), S(0, -27), S(7, -47), S(3, -96), S(-192, -145),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    CS(0, 0), S(12, 103), S(-21, 90), S(-13, 39), S(0, 10), S(9, 15), S(39, 12), S(18, 0),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    CS(0, 0), S(-183, -50), S(33, -3), S(-9, 44), S(13, 75), S(18, 100), S(38, 99), S(-10, 119),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-226, -223), S(-118, -60), S(-61, -8), S(-18, 25), S(28, 39), S(54, 77), S(91, 73), S(125, 76), S(171, 18),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-238, -283), S(-163, -98), S(-88, -36), S(-53, 13), S(-22, 43), S(-4, 64), S(14, 78), S(32, 83), S(52, 87), S(65, 83), S(91, 69), S(155, 23), S(187, 2), S(247, -30),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(-305, -228), S(-151, -81), S(-99, -17), S(-67, -7), S(-40, 16), S(-25, 38), S(-7, 50), S(11, 56), S(28, 68), S(46, 77), S(64, 79), S(77, 81), S(98, 84), S(109, 70), S(255, -56),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-970, -879), S(-254, -656), S(-158, -523), S(-103, -314), S(-95, -106), S(-56, -1), S(-52, 108), S(-27, 122), S(-22, 177), S(-10, 203), S(-1, 227), S(3, 242), S(22, 234), S(33, 246), S(40, 240), S(53, 236), S(60, 228), S(59, 235), S(85, 190), S(107, 154), S(122, 135), S(165, 69), S(178, 61), S(337, -112), S(372, -155), S(615, -311), S(470, -245), S(793, -417),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(447, 2), S(102, -119), S(-2, -28), S(-16, 9), S(-44, 12), S(-78, 18), S(-57, 19), S(-66, 13), S(-67, -35),
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    CS(0, 0), S(88, -31), S(158, -78),
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    CS(0, 0), S(37, -6), S(137, -43),
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    CS(0, 0), S(68, -48), S(53, -64), S(104, -63), S(161, -129),
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    CS(0, 0), S(-37, 27), S(-55, 38), S(0, -8), S(162, -97), S(357, -238),
};

inline const PParam PAWN_THREAT_KNIGHT = S(240, 57);
inline const PParam PAWN_THREAT_BISHOP = S(205, 99);
inline const PParam PAWN_THREAT_ROOK   = S(199, 56);
inline const PParam PAWN_THREAT_QUEEN  = S(179, -62);

inline const PParam KNIGHT_THREAT_BISHOP = S(105, 72);
inline const PParam KNIGHT_THREAT_ROOK   = S(244, 5);
inline const PParam KNIGHT_THREAT_QUEEN  = S(156, -68);

inline const PParam BISHOP_THREAT_KNIGHT = S(110, 34);
inline const PParam BISHOP_THREAT_ROOK   = S(244, 55);
inline const PParam BISHOP_THREAT_QUEEN  = S(192, 48);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(1, -6), S(-1, 0), S(0, -11), S(-5, -21), S(-11, -26), S(-17, -33), S(-18, -40), S(-24, -37), S(-34, -43),
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(111, 162),    S(101, 209),    S(169, 174),    S(231, 56),     S(177, 50),     S(162, 115),    S(59, 139),     S(118, 115),    //
    S(79, 44),      S(192, 72),     S(168, 15),     S(169, -42),    S(123, -59),    S(67, -10),     S(28, 36),      S(-21, 40),     //
    S(-1, 12),      S(18, 15),      S(34, -28),     S(21, -42),     S(4, -46),      S(-38, -40),    S(-78, 7),      S(-104, 28),    //
    S(-27, -36),    S(-9, -8),      S(-14, -41),    S(-32, -38),    S(-58, -47),    S(-78, -39),    S(-129, 10),    S(-147, -1),    //
    S(-29, -65),    S(34, -63),     S(-16, -19),    S(-47, -17),    S(-67, -26),    S(-107, -27),   S(-125, -14),   S(-149, -20),   //
    S(-19, -59),    S(111, -54),    S(66, -18),     S(8, 0),        S(-29, -12),    S(-65, -17),    S(-90, 6),      S(-128, -6),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-400, -159),  S(-350, 59),    S(-461, 234),   S(-126, 67),    S(-256, 92),    S(-340, 98),    S(-573, 85),    S(-545, -17),   //
    S(0, -2),       S(65, 9),       S(167, -57),    S(112, 8),      S(115, 14),     S(52, -9),      S(-7, 10),      S(-26, -35),    //
    S(57, -29),     S(99, 16),      S(191, 8),      S(142, 30),     S(141, 20),     S(61, 28),      S(49, 3),       S(-45, 11),     //
    S(111, 5),      S(101, 27),     S(132, 33),     S(110, 60),     S(119, 46),     S(86, 40),      S(60, -1),      S(35, 4),       //
    S(100, -14),    S(126, -17),    S(120, 8),      S(92, 25),      S(84, 33),      S(80, 27),      S(53, 0),       S(39, -55),     //
    S(12, -23),     S(43, -35),     S(36, -14),     S(48, 29),      S(55, 26),      S(-2, 3),       S(3, -35),      S(-36, -41),    //
    S(14, -8),      S(35, -36),     S(19, -30),     S(19, -11),     S(6, -17),      S(-21, -38),    S(-10, -52),    S(-67, -121),   //
    S(-34, -60),    S(3, -16),      S(21, -39),     S(30, -33),     S(22, -25),     S(-24, -55),    S(-37, -31),    S(-86, -84),    //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-167, 80),    S(-187, 61),    S(-429, 88),    S(-309, 101),   S(-260, 104),   S(-428, 130),   S(-169, 107),   S(-122, 82),    //
    S(5, -32),      S(-10, 46),     S(8, 27),       S(-11, 31),     S(-35, 46),     S(0, 38),       S(-20, 28),     S(-57, 29),     //
    S(35, 24),      S(82, 14),      S(161, 23),     S(89, 22),      S(64, 25),      S(40, 37),      S(97, 9),       S(-4, 26),      //
    S(52, -20),     S(65, 11),      S(101, 15),     S(100, 40),     S(106, 40),     S(44, 38),      S(32, 14),      S(-15, 20),     //
    S(53, -49),     S(61, -10),     S(68, 6),       S(68, 30),      S(60, 47),      S(20, 38),      S(4, 0),        S(0, -43),      //
    S(67, -39),     S(113, -20),    S(115, -8),     S(57, 36),      S(37, 40),      S(38, 36),      S(69, -6),      S(31, -36),     //
    S(53, -75),     S(103, -51),    S(73, -38),     S(45, -8),      S(36, -24),     S(37, -35),     S(19, -20),     S(38, -85),     //
    S(48, -56),     S(35, -9),      S(39, -3),      S(50, -37),     S(59, -48),     S(55, -7),      S(47, -34),     S(30, -37),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(105, 11),     S(171, 10),     S(100, 40),     S(100, 34),     S(107, 23),     S(56, 36),      S(63, 39),      S(71, 44),      //
    S(14, 69),      S(100, 44),     S(175, 21),     S(100, 64),     S(116, 52),     S(63, 61),      S(4, 80),       S(-4, 86),      //
    S(2, 46),       S(151, 4),      S(181, -1),     S(181, -5),     S(136, 4),      S(61, 46),      S(78, 33),      S(-40, 83),     //
    S(-28, 40),     S(48, 33),      S(79, 24),      S(102, -11),    S(71, 11),      S(9, 60),       S(-8, 59),      S(-79, 67),     //
    S(-92, -8),     S(-10, -3),     S(-25, 13),     S(-42, 14),     S(-47, 11),     S(-65, 51),     S(-95, 48),     S(-114, 37),    //
    S(-115, -28),   S(-40, -55),    S(-47, -25),    S(-66, -24),    S(-49, -42),    S(-99, 10),     S(-101, -6),    S(-124, -9),    //
    S(-177, -18),   S(-77, -78),    S(-53, -63),    S(-49, -61),    S(-56, -56),    S(-75, -40),    S(-95, -62),    S(-127, -46),   //
    S(-143, -16),   S(-112, -11),   S(-58, -47),    S(-33, -64),    S(-47, -49),    S(-60, -37),    S(-75, -45),    S(-93, -29),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(37, 41),      S(76, 7),       S(82, 13),      S(-38, 142),    S(33, 61),      S(-23, 88),     S(42, 2),       S(-22, 28),     //
    S(24, 80),      S(-48, 174),    S(-53, 230),    S(-140, 262),   S(-112, 207),   S(-123, 206),   S(-74, 111),    S(-38, 51),     //
    S(-17, 111),    S(72, 108),     S(13, 184),     S(-4, 195),     S(-45, 177),    S(-77, 176),    S(-7, 71),      S(-41, 43),     //
    S(51, 24),      S(55, 92),      S(18, 128),     S(6, 194),      S(-15, 178),    S(-24, 112),    S(13, 19),      S(0, -13),      //
    S(15, 52),      S(50, 14),      S(24, 83),      S(-15, 140),    S(-27, 127),    S(-21, 85),     S(-10, 6),      S(-7, -42),     //
    S(25, -106),    S(50, -62),     S(49, 7),       S(-2, 33),      S(9, -9),       S(12, -4),      S(23, -75),     S(0, -68),      //
    S(10, -212),    S(44, -312),    S(31, -175),    S(48, -102),    S(21, -83),     S(38, -154),    S(13, -92),     S(-4, -90),     //
    S(-45, -132),   S(15, -382),    S(12, -369),    S(38, -275),    S(41, -193),    S(45, -238),    S(31, -198),    S(-21, -122),   //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-137, -378),  S(99, -22),     S(-60, 30),     S(-168, 54),    S(20, -11),     S(20, -11),     S(20, -11),     S(20, -11),     //
    S(193, -114),   S(8, 142),      S(26, 128),     S(131, 58),     S(20, -11),     S(20, -11),     S(20, -11),     S(20, -11),     //
    S(-33, 56),     S(72, 134),     S(111, 103),    S(95, 58),      S(20, -11),     S(20, -11),     S(20, -11),     S(20, -11),     //
    S(-252, 80),    S(36, 95),      S(27, 92),      S(-25, 77),     S(20, -11),     S(20, -11),     S(20, -11),     S(20, -11),     //
    S(-226, 37),    S(-42, 68),     S(-25, 73),     S(-108, 109),   S(20, -11),     S(20, -11),     S(20, -11),     S(20, -11),     //
    S(-131, 7),     S(60, 10),      S(-29, 61),     S(-73, 87),     S(20, -11),     S(20, -11),     S(20, -11),     S(20, -11),     //
    S(84, -83),     S(137, -43),    S(49, 3),       S(-32, 46),     S(20, -11),     S(20, -11),     S(20, -11),     S(20, -11),     //
    S(-39, -112),   S(90, -102),    S(-9, -66),     S(-26, -65),    S(20, -11),     S(20, -11),     S(20, -11),     S(20, -11),     //
};
// Epoch duration: 61.8411s
// clang-format on
}  // namespace Clockwork
