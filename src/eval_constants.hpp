#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(266, 126);
inline const PParam KNIGHT_MAT = S(925, 328);
inline const PParam BISHOP_MAT = S(971, 350);
inline const PParam ROOK_MAT   = S(883, 305);
inline const PParam QUEEN_MAT  = S(1889, 361);
inline const PParam TEMPO_VAL  = S(77, 2);

inline const PParam BISHOP_PAIR_VAL   = S(154, 111);
inline const PParam ROOK_OPEN_VAL     = S(110, -16);
inline const PParam ROOK_SEMIOPEN_VAL = S(58, 13);

inline const PParam DOUBLED_PAWN_VAL = S(-39, -46);
inline const PParam ISOLATED_PAWN_VAL = S(-28, -12);

inline const PParam POTENTIAL_CHECKER_VAL = S(-77, -1);
inline const PParam OUTPOST_KNIGHT_VAL    = S(69, 30);
inline const PParam OUTPOST_BISHOP_VAL    = S(83, 12);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(48, 6);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(54, -18);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(55, 27);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(72, -71);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(17, 3), S(58, 17), S(87, 33), S(227, 81), S(666, 156), S(701, 438),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(78, 18), S(64, 14), S(91, 30), S(216, 77), S(561, 59),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-176, -86), S(-165, -77), S(-81, -18), S(34, 50), S(220, 147), S(524, 191),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(4, -21), S(29, -1), S(64, 11), S(123, 42), S(308, 61), S(495, 141),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(-5, -24), S(0, -1), S(-23, -18), S(-42, -36), S(-104, -71), S(-471, -97),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(138, 86), S(103, 65), S(38, 37), S(10, 19), S(18, 15), S(58, 16), S(38, 16),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-280, -12), S(31, 28), S(37, 38), S(93, 56), S(111, 63), S(110, 69), S(112, 51),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(151, -15), S(276, 122), S(343, 182), S(389, 204), S(441, 211), S(481, 235), S(517, 232), S(558, 238), S(577, 194),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(171, 5), S(254, 141), S(343, 162), S(390, 186), S(430, 204), S(458, 213), S(473, 226), S(495, 230), S(512, 236), S(532, 228), S(546, 230), S(572, 202), S(571, 212), S(565, 200),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(490, 166), S(467, 305), S(505, 312), S(532, 308), S(550, 312), S(562, 318), S(575, 320), S(589, 317), S(601, 322), S(615, 321), S(631, 320), S(642, 323), S(644, 326), S(643, 313), S(698, 240),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(849, 311), S(1079, 77), S(1121, 255), S(1155, 370), S(1176, 456), S(1188, 547), S(1199, 585), S(1212, 577), S(1218, 616), S(1227, 616), S(1238, 609), S(1250, 593), S(1259, 583), S(1261, 594), S(1268, 578), S(1266, 585), S(1269, 577), S(1275, 569), S(1284, 553), S(1295, 544), S(1297, 528), S(1303, 507), S(1298, 512), S(1279, 461), S(1246, 472), S(1239, 446), S(1180, 504), S(1173, 417),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(482, 10), S(93, -89), S(20, -6), S(13, 20), S(-9, 21), S(-20, 10), S(-14, 7), S(-13, -0), S(7, -42),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(10, -5), S(11, -13), S(10, -10), S(4, -5), S(1, 97),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(6, -3), S(4, -8), S(4, -7), S(4, -4), S(6, 300),
};

inline const PParam PAWN_THREAT_KNIGHT = S(282, 33);
inline const PParam PAWN_THREAT_BISHOP = S(281, 46);
inline const PParam PAWN_THREAT_ROOK   = S(287, 22);
inline const PParam PAWN_THREAT_QUEEN  = S(216, -134);

inline const PParam KNIGHT_THREAT_BISHOP = S(169, 52);
inline const PParam KNIGHT_THREAT_ROOK   = S(311, -12);
inline const PParam KNIGHT_THREAT_QUEEN  = S(211, -99);

inline const PParam BISHOP_THREAT_KNIGHT = S(143, 23);
inline const PParam BISHOP_THREAT_ROOK   = S(314, 23);
inline const PParam BISHOP_THREAT_QUEEN  = S(256, -0);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(-3, -20), S(-4, -9), S(-11, -14), S(-21, -19), S(-32, -22), S(-41, -25), S(-49, -32), S(-55, -32), S(-66, -32),
};

inline const PParam ROOK_LINEUP = S(20, 87);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(552, 158),    S(412, 261),    S(515, 231),    S(437, 133),    S(437, 115),    S(381, 161),    S(343, 194),    S(475, 128),    //
    S(226, 114),    S(269, 159),    S(252, 94),     S(192, 37),     S(160, 28),     S(161, 68),     S(165, 107),    S(138, 126),    //
    S(155, 108),    S(130, 110),    S(142, 66),     S(126, 46),     S(109, 47),     S(95, 74),      S(64, 108),     S(78, 121),     //
    S(98, 85),      S(99, 94),      S(113, 62),     S(105, 55),     S(80, 58),      S(76, 77),      S(26, 109),     S(30, 105),     //
    S(86, 67),      S(135, 64),     S(122, 89),     S(109, 77),     S(76, 77),      S(58, 80),      S(28, 96),      S(23, 90),      //
    S(104, 68),     S(216, 54),     S(211, 94),     S(147, 88),     S(107, 90),     S(92, 80),      S(63, 95),      S(48, 100),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-333, -71),   S(-65, 146),    S(-36, -79),    S(204, 134),    S(124, 155),    S(2, 130),      S(-155, 178),   S(-376, 13),    //
    S(166, 105),    S(229, 111),    S(298, 111),    S(308, 110),    S(304, 118),    S(221, 139),    S(185, 140),    S(149, 125),    //
    S(277, 86),     S(323, 99),     S(349, 126),    S(332, 148),    S(308, 165),    S(242, 170),    S(234, 130),    S(192, 133),    //
    S(352, 107),    S(380, 113),    S(376, 139),    S(375, 161),    S(372, 170),    S(317, 163),    S(291, 152),    S(276, 129),    //
    S(330, 113),    S(364, 100),    S(352, 143),    S(353, 156),    S(327, 160),    S(319, 154),    S(299, 125),    S(249, 142),    //
    S(248, 108),    S(285, 99),     S(290, 119),    S(306, 143),    S(305, 141),    S(252, 136),    S(229, 118),    S(178, 117),    //
    S(268, 100),    S(281, 102),    S(259, 100),    S(265, 124),    S(255, 123),    S(210, 100),    S(199, 118),    S(152, 74),     //
    S(170, 83),     S(222, 152),    S(247, 92),     S(263, 96),     S(248, 110),    S(200, 105),    S(187, 132),    S(116, 95),     //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(98, 238),     S(146, 205),    S(-60, 244),    S(40, 204),     S(76, 194),     S(49, 214),     S(134, 205),    S(116, 204),    //
    S(243, 115),    S(224, 185),    S(231, 171),    S(193, 184),    S(210, 188),    S(218, 191),    S(265, 172),    S(227, 159),    //
    S(358, 127),    S(372, 150),    S(363, 167),    S(333, 171),    S(299, 169),    S(315, 160),    S(324, 164),    S(309, 145),    //
    S(302, 150),    S(346, 145),    S(356, 165),    S(368, 187),    S(394, 183),    S(312, 172),    S(299, 156),    S(264, 156),    //
    S(321, 109),    S(343, 146),    S(361, 152),    S(385, 160),    S(362, 177),    S(329, 171),    S(284, 171),    S(260, 146),    //
    S(347, 125),    S(395, 125),    S(418, 143),    S(365, 165),    S(350, 154),    S(352, 160),    S(351, 143),    S(280, 149),    //
    S(312, 102),    S(422, 109),    S(386, 120),    S(348, 140),    S(326, 145),    S(326, 128),    S(323, 127),    S(322, 107),    //
    S(342, 105),    S(330, 131),    S(328, 165),    S(351, 107),    S(342, 124),    S(349, 160),    S(343, 138),    S(317, 130),    //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(621, 247),    S(662, 248),    S(643, 264),    S(618, 243),    S(585, 258),    S(561, 268),    S(568, 278),    S(560, 286),    //
    S(560, 276),    S(649, 272),    S(699, 258),    S(632, 264),    S(610, 285),    S(589, 289),    S(539, 305),    S(532, 303),    //
    S(513, 286),    S(634, 266),    S(660, 241),    S(607, 255),    S(567, 276),    S(546, 282),    S(548, 289),    S(506, 307),    //
    S(472, 274),    S(574, 256),    S(597, 242),    S(559, 251),    S(551, 266),    S(525, 282),    S(522, 274),    S(459, 297),    //
    S(408, 234),    S(497, 235),    S(495, 244),    S(463, 256),    S(453, 263),    S(460, 266),    S(418, 277),    S(396, 269),    //
    S(372, 201),    S(457, 203),    S(455, 226),    S(432, 233),    S(423, 240),    S(410, 256),    S(399, 233),    S(359, 243),    //
    S(251, 234),    S(393, 194),    S(423, 201),    S(422, 213),    S(407, 219),    S(400, 224),    S(374, 219),    S(342, 238),    //
    S(310, 228),    S(342, 243),    S(421, 200),    S(435, 202),    S(422, 214),    S(405, 228),    S(392, 225),    S(364, 242),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(914, 420),    S(944, 392),    S(918, 479),    S(894, 564),    S(892, 495),    S(893, 504),    S(881, 461),    S(821, 548),    //
    S(911, 506),    S(924, 540),    S(926, 557),    S(807, 604),    S(793, 650),    S(831, 661),    S(827, 599),    S(844, 527),    //
    S(937, 405),    S(965, 530),    S(934, 579),    S(871, 638),    S(835, 644),    S(869, 568),    S(879, 545),    S(879, 483),    //
    S(942, 391),    S(955, 528),    S(901, 601),    S(913, 590),    S(886, 652),    S(864, 594),    S(899, 517),    S(867, 539),    //
    S(943, 405),    S(950, 460),    S(937, 506),    S(906, 577),    S(893, 613),    S(891, 532),    S(877, 514),    S(884, 466),    //
    S(914, 331),    S(966, 380),    S(973, 456),    S(934, 475),    S(911, 520),    S(910, 549),    S(897, 502),    S(884, 397),    //
    S(864, 285),    S(910, 256),    S(928, 353),    S(958, 354),    S(931, 442),    S(911, 429),    S(876, 481),    S(886, 449),    //
    S(834, 302),    S(848, 148),    S(876, 186),    S(913, 286),    S(922, 385),    S(916, 320),    S(909, 319),    S(859, 425),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-224, -243),  S(550, 73),     S(441, 70),     S(237, -44),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(285, -71),    S(580, 61),     S(497, 84),     S(331, 6),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(377, -9),     S(640, 40),     S(618, 43),     S(455, -5),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(185, -30),    S(504, 12),     S(499, 24),     S(426, 5),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-15, -37),    S(318, -20),    S(381, -17),    S(346, -17),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-80, -11),    S(185, -8),     S(193, -1),     S(187, -18),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-78, -12),    S(22, 34),      S(-31, 48),     S(-65, 37),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-229, -26),   S(-94, -19),    S(-198, 19),    S(-212, -8),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(4, -22), S(-10, 11), S(-12, 4), S(-7, -7), S(-11, -6), S(-17, 9), S(-11, 12), }},
  {{ S(1, -16), S(-19, 4), S(-16, 1), S(-10, -12), S(-17, -2), S(-23, 34), S(-27, 79), }},
  {{ S(-5, -1), S(-16, 28), S(-11, 4), S(-10, -6), S(-16, 3), S(-21, 26), S(-18, 31), }},
  {{ S(-3, -13), S(-18, -8), S(-10, -15), S(-8, -20), S(-13, -14), S(-19, -4), S(-21, -6), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(9, 11), S(-8, 22), S(-4, 16), S(10, -5), S(13, -33),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(4, 2), S(-9, 45), S(-9, 30), S(-7, 19), S(-5, 3), S(-2, -4), S(-4, -2), }},
  {{ S(2, 6), S(-10, 44), S(-20, 43), S(-14, 26), S(-10, 11), S(-6, 6), S(-3, 10), }},
  {{ S(-3, 6), S(-14, 29), S(-18, 35), S(-11, 20), S(-6, 9), S(-5, 4), S(-5, 3), }},
  {{ S(-3, 5), S(-11, 20), S(-16, 22), S(-7, 14), S(-4, 10), S(-4, 3), S(-4, 3), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1525, 546, -42, 59
);

inline VParam WINNABLE_PAWNS = V(27);
inline VParam WINNABLE_BIAS = V(-249);

// Epoch duration: 7.3709s
// Epoch duration: 9.05276s
// clang-format on
}  // namespace Clockwork
