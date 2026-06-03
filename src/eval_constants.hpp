#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(192, 495);
inline const PParam KNIGHT_MAT = S(807, 1529);
inline const PParam BISHOP_MAT = S(852, 1624);
inline const PParam ROOK_MAT   = S(1013, 2563);
inline const PParam QUEEN_MAT  = S(2393, 3880);

inline const PParam TEMPO_VAL  = S(68, 49);

inline const PParam BISHOP_XRAY_PAWNS   = S(-12, -4);
inline const PParam BISHOP_PAIR_VAL   = S(64, 224);
inline const PParam ROOK_OPEN_VAL     = S(111, -3);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, 12);
inline const PParam MINOR_BEHIND_PAWN = S(15, 38);
inline const PParam RESTRICTED_SQUARES = S(-22, -3);

inline const PParam DOUBLED_PAWN_VAL = S(-20, -80);
inline const PParam ISOLATED_PAWN_VAL = S(-13, -36);

inline const PParam POTENTIAL_CHECKER_VAL = S(-45, -40);
inline const PParam OUTPOST_KNIGHT_VAL    = S(52, 41);
inline const PParam OUTPOST_BISHOP_VAL    = S(45, 35);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(37, 48);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(40, 3);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(25, 54);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(60, -48);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(14, -3), S(31, 35), S(59, 63), S(128, 193), S(418, 289), S(583, 610),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(45, 34), S(38, 23), S(54, 52), S(102, 165), S(403, 92),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-89, -264), S(-88, -230), S(-66, -93), S(-25, 41), S(70, 231), S(214, 400),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(25, -41), S(25, -3), S(14, 38), S(12, 116), S(76, 243), S(105, 440),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(14, -37), S(1, 11), S(-7, -21), S(-4, -58), S(-9, -164), S(-168, -329),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(14, 238), S(1, 191), S(-0, 115), S(9, 75), S(21, 76), S(60, 74), S(52, 53),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-135, -59), S(13, 4), S(1, 104), S(26, 153), S(37, 190), S(50, 195), S(33, 180),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-84, -374), S(-24, -144), S(5, -27), S(29, 19), S(58, 47), S(72, 83), S(91, 89), S(110, 97), S(128, 53),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-63, -284), S(-16, -118), S(33, -48), S(55, 3), S(78, 41), S(90, 69), S(96, 90), S(101, 109), S(106, 122), S(114, 128), S(124, 121), S(140, 106), S(145, 109), S(123, 86),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(106, -190), S(25, -13), S(46, 20), S(61, 34), S(72, 52), S(76, 66), S(81, 80), S(87, 84), S(92, 97), S(99, 102), S(105, 106), S(108, 112), S(110, 117), S(114, 107), S(142, 58),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(-208, -134), S(-61, -26), S(-35, 96), S(-14, 220), S(2, 260), S(9, 311), S(15, 344), S(21, 360), S(25, 385), S(27, 403), S(32, 411), S(35, 422), S(38, 426), S(39, 433), S(39, 436), S(35, 443), S(32, 443), S(35, 435), S(35, 433), S(40, 423), S(36, 409), S(40, 387), S(9, 416), S(-31, 398), S(-62, 400), S(-97, 423), S(-143, 447), S(-113, 362),
};

inline const PParam PAWN_THREAT_KNIGHT = S(193, 142);
inline const PParam PAWN_THREAT_BISHOP = S(170, 205);
inline const PParam PAWN_THREAT_ROOK   = S(191, 143);
inline const PParam PAWN_THREAT_QUEEN  = S(148, 17);

inline const std::array<PParam, 6> MINOR_THREAT = {
    S(8, 54), S(91, 89), S(103, 120), S(202, 120), S(160, 15), S(0, 0),
};
inline const std::array<PParam, 6> ROOK_THREAT = {
    S(9, 54), S(52, 92), S(73, 92), S(21, -8), S(172, -58), S(0, 0),
};
inline const PParam KING_THREAT  = S(-17, 154);
inline const PParam HANGING_PAWN  = S(30, 88);
inline const PParam HANGING_NON_PAWN  = S(66, 24);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(5, -17), S(4, -14), S(-0, -20), S(-5, -28), S(-11, -36), S(-16, -43), S(-18, -56), S(-23, -59), S(-27, -76),
};

inline const PParam ROOK_LINEUP = S(15, 79);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(184, 270),    S(46, 380),     S(65, 357),     S(135, 258),    S(191, 163),    S(122, 223),    S(89, 267),     S(227, 202),    //
    S(37, 69),      S(46, 112),     S(28, 53),      S(42, -21),     S(36, -58),     S(7, -15),      S(3, 41),       S(-22, 69),     //
    S(34, -7),      S(6, 3),        S(38, -44),     S(23, -67),     S(14, -85),     S(-5, -69),     S(-41, -32),    S(-41, 5),      //
    S(-8, -68),     S(-32, -37),    S(2, -61),      S(-8, -72),     S(-29, -82),    S(-39, -70),    S(-86, -47),    S(-77, -46),    //
    S(-12, -105),   S(22, -104),    S(8, -47),      S(-7, -52),     S(-35, -63),    S(-51, -69),    S(-80, -65),    S(-81, -68),    //
    S(11, -103),    S(84, -94),     S(70, -44),     S(26, -18),     S(-3, -41),     S(-23, -60),    S(-49, -54),    S(-60, -52),    //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-253, -495),  S(-239, 41),    S(-258, -91),   S(26, 21),      S(-78, 36),     S(-239, 55),    S(-358, 104),   S(-336, -380),  //
    S(-4, -16),     S(-4, 31),      S(101, 7),      S(78, 53),      S(92, 35),      S(53, 16),      S(-16, 21),     S(-29, 10),     //
    S(45, -29),     S(34, 30),      S(72, 46),      S(77, 62),      S(66, 61),      S(25, 52),      S(12, 25),      S(-36, 10),     //
    S(97, 20),      S(93, 40),      S(103, 60),     S(105, 103),    S(110, 103),    S(76, 67),      S(53, 33),      S(40, 28),      //
    S(75, 15),      S(105, -0),     S(100, 48),     S(93, 77),      S(80, 78),      S(77, 60),      S(64, 9),       S(23, 28),      //
    S(17, -36),     S(45, -18),     S(48, 21),      S(63, 54),      S(57, 51),      S(29, 16),      S(16, -9),      S(-25, -36),    //
    S(38, -21),     S(43, -17),     S(26, -14),     S(33, 13),      S(30, 8),       S(4, -41),      S(-28, -2),     S(-36, -95),    //
    S(-41, -77),    S(8, -23),      S(28, -42),     S(46, -33),     S(26, -11),     S(-10, -47),    S(-19, -25),    S(-61, -132),   //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-115, 22),    S(-164, 96),    S(-426, 187),   S(-262, 85),    S(-280, 130),   S(-287, 140),   S(-189, 106),   S(-114, 57),    //
    S(-26, -14),    S(-75, 90),     S(-35, 49),     S(-73, 67),     S(-64, 64),     S(-30, 39),     S(-4, 31),      S(-40, 19),     //
    S(37, 24),      S(10, 66),      S(28, 71),      S(24, 60),      S(19, 39),      S(18, 44),      S(12, 45),      S(19, 16),      //
    S(15, -0),      S(56, 15),      S(64, 33),      S(75, 55),      S(104, 39),     S(46, 21),      S(52, -8),      S(5, -3),       //
    S(51, -53),     S(49, -3),      S(80, 9),       S(97, 20),      S(78, 38),      S(70, 19),      S(22, 7),       S(22, -32),     //
    S(62, -34),     S(85, -29),     S(99, 1),       S(68, 28),      S(65, 10),      S(63, 15),      S(71, -9),      S(22, -19),     //
    S(34, -87),     S(116, -54),    S(73, -19),     S(51, -3),      S(34, 1),       S(54, -43),     S(52, -50),     S(48, -61),     //
    S(51, -80),     S(22, -17),     S(25, -3),      S(50, -32),     S(33, -22),     S(39, 13),      S(48, -21),     S(45, -58),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(183, 78),     S(217, 97),     S(163, 127),    S(150, 105),    S(210, 62),     S(171, 81),     S(193, 84),     S(166, 90),     //
    S(94, 135),     S(153, 130),    S(207, 95),     S(150, 98),     S(196, 77),     S(151, 104),    S(102, 132),    S(97, 137),     //
    S(63, 129),     S(187, 76),     S(211, 58),     S(188, 39),     S(185, 61),     S(131, 94),     S(132, 105),    S(74, 149),     //
    S(36, 97),      S(104, 99),     S(130, 72),     S(107, 74),     S(135, 62),     S(97, 104),     S(92, 110),     S(30, 139),     //
    S(7, 36),       S(78, 44),      S(67, 60),      S(41, 66),      S(51, 73),      S(38, 100),     S(16, 100),     S(-6, 102),     //
    S(18, -32),     S(90, -13),     S(80, 11),      S(60, 16),      S(72, 22),      S(39, 57),      S(43, 35),      S(-2, 45),      //
    S(-69, -28),    S(65, -70),     S(69, -33),     S(57, -0),      S(61, -3),      S(46, 12),      S(34, -2),      S(5, 4),        //
    S(-6, -54),     S(8, -12),      S(80, -30),     S(90, -33),     S(89, -25),     S(67, -0),      S(64, -12),     S(45, -5),      //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(57, 173),     S(138, 129),    S(42, 276),     S(-10, 367),    S(31, 318),     S(62, 233),     S(62, 150),     S(-7, 208),     //
    S(51, 181),     S(48, 246),     S(11, 317),     S(-128, 444),   S(-74, 410),    S(-22, 306),    S(28, 166),     S(15, 161),     //
    S(-1, 249),     S(28, 282),     S(-8, 356),     S(-53, 400),    S(-34, 384),    S(21, 261),     S(56, 155),     S(50, 100),     //
    S(16, 154),     S(2, 255),      S(-31, 304),    S(-55, 384),    S(-36, 380),    S(2, 238),      S(43, 148),     S(23, 122),     //
    S(7, 147),      S(-2, 177),     S(-17, 229),    S(-41, 294),    S(-29, 316),    S(-4, 239),     S(6, 164),      S(34, 74),      //
    S(7, 47),       S(29, 65),      S(22, 145),     S(0, 161),      S(11, 164),     S(12, 197),     S(33, 121),     S(24, 84),      //
    S(-19, -103),   S(11, -96),     S(13, -18),     S(36, 19),      S(30, 62),      S(35, 35),      S(8, 72),       S(25, 52),      //
    S(-31, -76),    S(-14, -227),   S(16, -237),    S(31, -122),    S(38, -43),     S(39, -77),     S(35, -50),     S(8, -4),       //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(134, -597),   S(492, -4),     S(297, 170),    S(16, 133),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(355, -187),   S(382, 100),    S(205, 172),    S(22, 134),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(189, 41),     S(226, 156),    S(122, 186),    S(-34, 166),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(6, 13),       S(92, 114),     S(-28, 169),    S(-84, 180),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-118, -0),    S(-30, 76),     S(-95, 145),    S(-172, 190),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-51, -31),    S(11, 36),      S(-71, 116),    S(-123, 159),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(21, -80),     S(27, -4),      S(-36, 58),     S(-110, 111),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-21, -193),   S(2, -79),      S(-81, -14),    S(-76, -36),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
}; 

inline const PParam KS_NO_QUEEN = S(-86, -438);

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(8, -5), S(10, 11), S(10, 4), S(5, 1), S(3, -10),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -7), S(4, 4), S(2, 3), S(3, 0), S(3, -1),
};

inline const PParam KS_FLANK_ATTACK = S(3, -2);
inline const PParam KS_FLANK_DEFENSE = S(-3, 0);
inline const PParam KS_FLANK_DOUBLE_ATTACK   = S(2, -0);
inline const PParam KS_FLANK_DOUBLE_DEFENSE  = S(-2, 2);

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(17, -20), S(-7, 5), S(-3, -10), S(8, -17), S(12, -29), S(5, -52), S(-6, -45), }},
  {{ S(1, 1), S(-22, 0), S(-17, 7), S(-10, 15), S(-7, 7), S(-9, -19), S(-19, -26), }},
  {{ S(-2, -2), S(-10, -10), S(-12, 18), S(-7, 17), S(-6, 10), S(-7, -17), S(-23, -40), }},
  {{ S(6, 6), S(-10, 16), S(-7, 36), S(-3, 41), S(-3, 36), S(4, 13), S(7, -14), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(8, 26), S(-5, 3), S(-8, 10), S(-6, 27), S(2, 46),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(5, 10), S(-44, -140), S(-10, -41), S(-0, 4), S(-2, 3), S(-6, 10), S(-5, 8), }},
  {{ S(9, -1), S(-25, -148), S(-6, -29), S(-4, 1), S(-0, -3), S(-7, 4), S(2, 2), }},
  {{ S(-0, 10), S(-10, -80), S(6, -15), S(-1, 9), S(-4, 10), S(-10, 18), S(-6, 20), }},
  {{ S(1, 1), S(2, -55), S(-2, 30), S(-3, 26), S(-6, 10), S(-12, 12), S(-10, 20), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1410, 873, -20, -3
);

inline VParam WINNABLE_PAWNS = V(-20);
inline VParam WINNABLE_SYM = V(100);
inline VParam WINNABLE_ASYM = V(83);
inline VParam WINNABLE_PAWN_ENDGAME = V(108);
inline VParam WINNABLE_BIAS = V(-370);

// Epoch duration: 7.32325s
// clang-format on
}  // namespace Clockwork
