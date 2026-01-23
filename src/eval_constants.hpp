#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(174, 139);
inline const PParam KNIGHT_MAT = S(656, 306);
inline const PParam BISHOP_MAT = S(719, 313);
inline const PParam ROOK_MAT   = S(701, 365);
inline const PParam QUEEN_MAT  = S(2125, -671);
inline const PParam TEMPO_VAL  = S(64, 11);

inline const PParam BISHOP_PAIR_VAL   = S(77, 147);
inline const PParam ROOK_OPEN_VAL     = S(111, -25);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, 8);

inline const PParam DOUBLED_PAWN_VAL = S(-32, -63);

inline const PParam POTENTIAL_CHECKER_VAL = S(-61, -3);
inline const PParam OUTPOST_KNIGHT_VAL    = S(47, 44);
inline const PParam OUTPOST_BISHOP_VAL    = S(57, 26);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(43, 10);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(53, -21);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(34, 42);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(69, -60);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(21, 20), S(53, 36), S(71, 66), S(181, 142), S(496, 203), S(647, 615),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(64, 43), S(57, 32), S(75, 59), S(177, 113), S(572, 0),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-86, -95), S(-79, -78), S(-46, -1), S(26, 77), S(111, 197), S(323, 254),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(29, -32), S(33, -6), S(25, 23), S(21, 72), S(80, 142), S(186, 221),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(14, -28), S(1, 14), S(-7, -14), S(-12, -35), S(-21, -89), S(-251, -127),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(36, 80), S(1, 79), S(-8, 39), S(-11, 17), S(-7, 19), S(31, 15), S(-4, 0),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-284, -40), S(-10, 2), S(-4, 36), S(22, 72), S(30, 90), S(44, 97), S(21, 86),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(102, -63), S(197, 99), S(250, 177), S(287, 210), S(332, 225), S(358, 260), S(393, 255), S(427, 259), S(474, 197),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(126, -85), S(189, 93), S(252, 144), S(278, 196), S(308, 227), S(327, 248), S(335, 266), S(352, 273), S(363, 285), S(382, 274), S(402, 264), S(456, 222), S(457, 222), S(517, 178),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(336, 128), S(269, 284), S(297, 308), S(315, 315), S(329, 328), S(334, 340), S(341, 350), S(352, 350), S(360, 357), S(370, 359), S(383, 358), S(393, 357), S(400, 355), S(425, 332), S(540, 237),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(684, -130), S(756, -48), S(806, -12), S(834, 84), S(845, 143), S(863, 172), S(867, 212), S(877, 214), S(881, 235), S(887, 242), S(891, 253), S(897, 257), S(908, 245), S(912, 248), S(914, 247), S(917, 244), S(921, 237), S(920, 236), S(931, 216), S(950, 197), S(966, 175), S(982, 140), S(1008, 119), S(1132, -0), S(1167, -45), S(1175, -64), S(1082, -13), S(1130, -62),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(511, -163), S(127, -73), S(38, 6), S(25, 28), S(2, 14), S(-19, 10), S(-20, 7), S(-33, 2), S(-29, -43),
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(-7, 32), S(18, 38), S(28, 40),
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(-5, 35), S(0, 15), S(4, -5),
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(-8, 43), S(12, 57), S(10, 64), S(14, 80), S(385, 68),
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(439, 516), S(314, 308), S(190, 171), S(71, 74), S(-37, 58), S(-161, 45),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(12, 4), S(2, -2), S(12, 22), S(-2, -12), S(124, 175),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(2, 2), S(8, 1), S(7, -0), S(6, 0), S(7, 259),
};

inline const PParam PAWN_THREAT_KNIGHT = S(242, 62);
inline const PParam PAWN_THREAT_BISHOP = S(216, 115);
inline const PParam PAWN_THREAT_ROOK   = S(205, 109);
inline const PParam PAWN_THREAT_QUEEN  = S(186, -49);

inline const PParam KNIGHT_THREAT_BISHOP = S(117, 78);
inline const PParam KNIGHT_THREAT_ROOK   = S(256, 19);
inline const PParam KNIGHT_THREAT_QUEEN  = S(158, -27);

inline const PParam BISHOP_THREAT_KNIGHT = S(113, 51);
inline const PParam BISHOP_THREAT_ROOK   = S(238, 78);
inline const PParam BISHOP_THREAT_QUEEN  = S(197, 62);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(2, -20), S(-4, -10), S(-3, -20), S(-9, -27), S(-15, -32), S(-21, -34), S(-25, -43), S(-31, -44), S(-40, -49),
};

inline const PParam ROOK_LINEUP = S(11, 78);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(313, 254),    S(194, 322),    S(330, 293),    S(302, 230),    S(341, 177),    S(259, 257),    S(212, 274),    S(320, 223),    //
    S(144, 156),    S(208, 185),    S(209, 129),    S(193, 91),     S(173, 73),     S(139, 118),    S(124, 156),    S(86, 167),     //
    S(96, 137),     S(103, 138),    S(130, 82),     S(119, 74),     S(104, 73),     S(71, 99),      S(50, 138),     S(34, 153),     //
    S(64, 109),     S(79, 126),     S(99, 82),      S(86, 86),      S(61, 87),      S(48, 104),     S(8, 148),      S(-3, 128),     //
    S(58, 82),      S(119, 82),     S(93, 107),     S(81, 108),     S(52, 110),     S(26, 113),     S(10, 129),     S(-6, 111),     //
    S(76, 84),      S(184, 93),     S(163, 137),    S(112, 133),    S(84, 123),     S(57, 127),     S(45, 139),     S(12, 128),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-252, -42),   S(-199, 237),   S(-416, 429),   S(97, 185),     S(22, 192),     S(-100, 181),   S(-357, 256),   S(-312, 41),    //
    S(127, 167),    S(180, 159),    S(268, 104),    S(246, 129),    S(236, 147),    S(180, 146),    S(155, 170),    S(119, 159),    //
    S(210, 125),    S(251, 136),    S(283, 160),    S(257, 170),    S(233, 179),    S(170, 198),    S(175, 151),    S(140, 161),    //
    S(280, 158),    S(288, 152),    S(278, 177),    S(267, 199),    S(269, 204),    S(235, 196),    S(233, 166),    S(221, 153),    //
    S(268, 147),    S(306, 127),    S(283, 163),    S(273, 185),    S(254, 188),    S(252, 185),    S(248, 145),    S(211, 149),    //
    S(212, 116),    S(246, 115),    S(228, 148),    S(249, 170),    S(248, 168),    S(198, 153),    S(193, 126),    S(159, 106),    //
    S(223, 127),    S(251, 110),    S(224, 116),    S(220, 136),    S(207, 140),    S(184, 93),     S(185, 119),    S(143, 59),     //
    S(165, 90),     S(200, 136),    S(226, 118),    S(244, 103),    S(226, 117),    S(177, 97),     S(170, 120),    S(119, 52),     //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(52, 260),     S(17, 250),     S(-281, 324),   S(-153, 294),   S(-117, 299),   S(-202, 327),   S(-20, 299),    S(58, 269),     //
    S(174, 193),    S(113, 252),    S(147, 208),    S(78, 255),     S(65, 260),     S(127, 241),    S(155, 226),    S(137, 224),    //
    S(252, 194),    S(276, 193),    S(262, 225),    S(220, 222),    S(189, 222),    S(190, 230),    S(232, 215),    S(212, 198),    //
    S(221, 184),    S(247, 195),    S(259, 203),    S(244, 240),    S(269, 233),    S(201, 220),    S(217, 199),    S(174, 202),    //
    S(244, 153),    S(253, 177),    S(262, 198),    S(273, 214),    S(256, 227),    S(226, 220),    S(178, 206),    S(190, 175),    //
    S(263, 151),    S(299, 164),    S(314, 180),    S(264, 211),    S(256, 197),    S(256, 199),    S(266, 176),    S(203, 180),    //
    S(248, 114),    S(331, 130),    S(292, 147),    S(260, 170),    S(240, 176),    S(248, 149),    S(247, 153),    S(241, 116),    //
    S(251, 117),    S(234, 167),    S(243, 185),    S(262, 159),    S(267, 144),    S(262, 187),    S(256, 147),    S(235, 141),    //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(449, 306),    S(498, 306),    S(438, 335),    S(436, 319),    S(444, 314),    S(390, 329),    S(417, 329),    S(405, 337),    //
    S(371, 343),    S(435, 333),    S(505, 312),    S(440, 335),    S(446, 335),    S(406, 343),    S(328, 372),    S(327, 373),    //
    S(339, 340),    S(471, 301),    S(488, 297),    S(467, 301),    S(436, 307),    S(389, 331),    S(385, 336),    S(312, 369),    //
    S(321, 329),    S(401, 323),    S(440, 304),    S(414, 307),    S(405, 320),    S(363, 341),    S(343, 344),    S(284, 359),    //
    S(282, 283),    S(361, 286),    S(340, 307),    S(328, 300),    S(315, 312),    S(301, 342),    S(270, 334),    S(245, 331),    //
    S(256, 249),    S(325, 247),    S(324, 276),    S(304, 272),    S(306, 268),    S(277, 306),    S(268, 284),    S(224, 297),    //
    S(170, 260),    S(295, 208),    S(307, 238),    S(310, 244),    S(300, 246),    S(283, 258),    S(260, 242),    S(234, 254),    //
    S(218, 235),    S(232, 261),    S(300, 249),    S(319, 236),    S(299, 252),    S(289, 260),    S(276, 249),    S(258, 269),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(581, 260),    S(668, 192),    S(653, 230),    S(592, 333),    S(613, 293),    S(585, 298),    S(616, 223),    S(504, 308),    //
    S(620, 280),    S(566, 384),    S(559, 417),    S(435, 483),    S(443, 474),    S(490, 436),    S(520, 342),    S(535, 317),    //
    S(581, 319),    S(658, 330),    S(588, 409),    S(540, 440),    S(498, 443),    S(524, 392),    S(584, 294),    S(566, 252),    //
    S(630, 258),    S(636, 325),    S(586, 382),    S(556, 448),    S(538, 455),    S(545, 371),    S(605, 270),    S(596, 231),    //
    S(619, 262),    S(627, 290),    S(598, 349),    S(567, 405),    S(569, 394),    S(567, 344),    S(582, 267),    S(599, 214),    //
    S(605, 202),    S(638, 235),    S(636, 302),    S(604, 288),    S(597, 281),    S(604, 293),    S(605, 231),    S(592, 192),    //
    S(585, 101),    S(626, 79),     S(628, 140),    S(639, 181),    S(623, 206),    S(617, 177),    S(588, 212),    S(594, 180),    //
    S(550, 121),    S(592, -49),    S(596, -39),    S(621, 56),     S(625, 131),    S(630, 71),     S(620, 87),     S(581, 120),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-348, -240),  S(88, 85),      S(159, 13),     S(-82, -12),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(202, -98),    S(274, 81),     S(145, 113),    S(100, 40),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(136, 22),     S(288, 100),    S(195, 109),    S(81, 79),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-115, 36),    S(177, 88),     S(20, 111),     S(-3, 104),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-157, 13),    S(100, 61),     S(18, 87),      S(-74, 126),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-67, -16),    S(147, 20),     S(17, 62),      S(-11, 87),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(24, -67),     S(109, 2),      S(9, 22),       S(-45, 47),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-93, -120),   S(15, -60),     S(-91, -61),    S(-78, -105),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(-47, -31), S(-63, -32), S(-70, -32), S(-61, -33), S(-62, -33), S(-69, -32), S(-61, -32), }},
  {{ S(-65, -37), S(-93, -39), S(-90, -40), S(-80, -43), S(-80, -41), S(-90, -39), S(-92, -37), }},
  {{ S(-71, -45), S(-82, -44), S(-81, -48), S(-76, -50), S(-76, -49), S(-79, -48), S(-79, -44), }},
  {{ S(-62, -34), S(-80, -42), S(-74, -41), S(-69, -40), S(-70, -40), S(-74, -41), S(-77, -39), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(-38, -25), S(-65, -31), S(-65, -32), S(-57, -31), S(-67, -34),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(-52, -32), S(-66, -31), S(-65, -30), S(-63, -31), S(-64, -32), S(-62, -30), S(-65, -31), }},
  {{ S(-51, -31), S(-62, -30), S(-69, -30), S(-63, -33), S(-64, -33), S(-68, -30), S(-62, -30), }},
  {{ S(-56, -34), S(-64, -33), S(-65, -31), S(-60, -32), S(-60, -33), S(-65, -31), S(-63, -32), }},
  {{ S(-57, -35), S(-70, -32), S(-71, -34), S(-61, -32), S(-59, -32), S(-70, -34), S(-72, -32), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        S(874, 1999), S(-104, 40)
);
// Epoch duration: 5.80449s

// Epoch duration: 5.20419s
// clang-format on
}  // namespace Clockwork
