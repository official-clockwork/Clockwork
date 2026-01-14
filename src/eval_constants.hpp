#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(174, 156);
inline const PParam KNIGHT_MAT = S(510, 311);
inline const PParam BISHOP_MAT = S(458, 270);
inline const PParam ROOK_MAT   = S(542, 410);
inline const PParam QUEEN_MAT  = S(1077, 560);
inline const PParam TEMPO_VAL  = S(63, 14);

inline const PParam BISHOP_PAIR_VAL   = S(77, 163);
inline const PParam ROOK_OPEN_VAL     = S(114, -26);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, 18);

inline const PParam DOUBLED_PAWN_VAL = S(-38, -72);

inline const PParam POTENTIAL_CHECKER_VAL = S(-69, -10);
inline const PParam OUTPOST_KNIGHT_VAL    = S(51, 40);
inline const PParam OUTPOST_BISHOP_VAL    = S(60, 26);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(49, 4);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(58, -25);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(37, 35);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(70, -52);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(23, 10), S(59, 32), S(74, 69), S(183, 152), S(483, 221), S(677, 677),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(72, 36), S(61, 33), S(75, 64), S(179, 115), S(598, -14),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-83, -95), S(-81, -75), S(-55, 5), S(29, 73), S(105, 195), S(304, 275),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(42, -36), S(36, -6), S(29, 18), S(26, 64), S(95, 131), S(209, 213),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(10, -33), S(-2, 10), S(-3, -19), S(-7, -40), S(-13, -90), S(-266, -110),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(18, 108), S(-9, 88), S(-11, 35), S(-7, 8), S(3, 9), S(46, 5), S(8, -3),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-294, -10), S(-33, 24), S(-10, 49), S(37, 60), S(40, 79), S(48, 85), S(20, 81),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(11, -59), S(110, 94), S(167, 164), S(204, 193), S(251, 203), S(280, 236), S(316, 228), S(353, 230), S(399, 169),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-14, -122), S(55, 36), S(117, 79), S(144, 127), S(173, 156), S(191, 176), S(199, 193), S(218, 197), S(229, 209), S(249, 198), S(272, 187), S(331, 144), S(337, 142), S(406, 97),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(279, 101), S(203, 261), S(231, 280), S(249, 284), S(262, 294), S(268, 306), S(276, 313), S(286, 312), S(294, 318), S(305, 319), S(318, 318), S(327, 317), S(334, 317), S(353, 296), S(461, 201),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(458, 111), S(522, 233), S(571, 268), S(599, 362), S(611, 422), S(627, 454), S(630, 497), S(639, 501), S(642, 526), S(648, 535), S(652, 549), S(657, 552), S(669, 541), S(674, 543), S(678, 540), S(682, 535), S(687, 528), S(686, 527), S(698, 507), S(717, 488), S(732, 468), S(748, 435), S(776, 415), S(895, 303), S(947, 250), S(942, 242), S(858, 290), S(962, 217),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(657, -328), S(242, -191), S(120, -88), S(81, -37), S(31, -21), S(-24, 5), S(-72, 42), S(-124, 68), S(-160, 55),
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(233, 231), S(352, 184), S(456, 122),
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(471, 377), S(289, 238), S(163, 72),
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(316, 320), S(439, 315), S(484, 321), S(591, 369), S(751, 328),
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(876, 748), S(590, 611), S(333, 463), S(143, 258), S(85, -7), S(18, -292),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(-119, 60), S(27, -18), S(-223, -130), S(60, 36), S(-242, -153),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(-28, 20), S(-21, 20), S(-21, 16), S(-15, 9), S(-21, -13),
};

inline const PParam PAWN_THREAT_KNIGHT = S(245, 60);
inline const PParam PAWN_THREAT_BISHOP = S(218, 116);
inline const PParam PAWN_THREAT_ROOK   = S(203, 99);
inline const PParam PAWN_THREAT_QUEEN  = S(187, -44);

inline const PParam KNIGHT_THREAT_BISHOP = S(118, 73);
inline const PParam KNIGHT_THREAT_ROOK   = S(256, 15);
inline const PParam KNIGHT_THREAT_QUEEN  = S(156, -19);

inline const PParam BISHOP_THREAT_KNIGHT = S(115, 38);
inline const PParam BISHOP_THREAT_ROOK   = S(238, 72);
inline const PParam BISHOP_THREAT_QUEEN  = S(199, 52);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(3, -20), S(-3, -8), S(-3, -18), S(-9, -25), S(-14, -32), S(-21, -35), S(-24, -46), S(-29, -47), S(-37, -52),
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(306, 277),    S(197, 345),    S(313, 306),    S(292, 249),    S(340, 180),    S(249, 251),    S(200, 278),    S(311, 231),    //
    S(159, 173),    S(217, 215),    S(193, 167),    S(164, 125),    S(146, 98),     S(114, 136),    S(95, 175),     S(66, 184),     //
    S(108, 140),    S(105, 164),    S(103, 128),    S(91, 103),     S(80, 89),      S(46, 114),     S(29, 145),     S(15, 161),     //
    S(95, 95),      S(108, 131),    S(95, 109),     S(82, 94),      S(55, 85),      S(36, 107),     S(-2, 145),     S(-14, 129),    //
    S(89, 67),      S(155, 70),     S(87, 122),     S(77, 118),     S(47, 111),     S(10, 114),     S(-6, 127),     S(-21, 113),    //
    S(91, 75),      S(210, 79),     S(154, 118),    S(105, 146),    S(75, 131),     S(50, 110),     S(30, 133),     S(-0, 128),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-284, -49),   S(-219, 185),   S(-456, 437),   S(89, 149),     S(16, 155),     S(-116, 135),   S(-374, 196),   S(-343, 33),    //
    S(111, 121),    S(162, 120),    S(243, 81),     S(215, 118),    S(210, 130),    S(143, 124),    S(125, 132),    S(96, 106),     //
    S(180, 91),     S(197, 135),    S(249, 152),    S(210, 168),    S(181, 178),    S(125, 185),    S(133, 126),    S(105, 121),    //
    S(240, 132),    S(222, 151),    S(226, 176),    S(199, 208),    S(217, 198),    S(187, 183),    S(188, 139),    S(185, 115),    //
    S(230, 118),    S(267, 108),    S(242, 152),    S(226, 175),    S(213, 172),    S(213, 161),    S(213, 110),    S(175, 112),    //
    S(176, 92),     S(204, 90),     S(187, 126),    S(207, 150),    S(209, 144),    S(157, 134),    S(157, 99),     S(123, 82),     //
    S(185, 84),     S(200, 72),     S(181, 87),     S(179, 108),    S(166, 114),    S(145, 70),     S(151, 81),     S(110, 18),     //
    S(127, 63),     S(163, 121),    S(187, 74),     S(205, 61),     S(189, 81),     S(139, 63),     S(129, 98),     S(78, 37),      //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-36, 183),    S(-70, 166),    S(-364, 241),   S(-228, 208),   S(-195, 218),   S(-293, 251),   S(-109, 219),   S(-29, 193),    //
    S(77, 112),     S(23, 172),     S(58, 130),     S(-12, 180),    S(-26, 186),    S(33, 161),     S(62, 144),     S(41, 135),     //
    S(141, 125),    S(172, 121),    S(163, 155),    S(125, 147),    S(94, 147),     S(93, 155),     S(135, 133),    S(114, 111),    //
    S(124, 104),    S(140, 125),    S(161, 136),    S(140, 175),    S(168, 163),    S(105, 147),    S(113, 120),    S(81, 113),     //
    S(139, 81),     S(148, 109),    S(156, 128),    S(163, 150),    S(151, 156),    S(116, 147),    S(77, 129),     S(84, 96),      //
    S(154, 86),     S(190, 91),     S(207, 107),    S(155, 144),    S(146, 127),    S(151, 126),    S(161, 101),    S(99, 108),     //
    S(139, 35),     S(223, 55),     S(186, 68),     S(152, 95),     S(134, 106),    S(141, 78),     S(142, 85),     S(132, 45),     //
    S(142, 46),     S(128, 96),     S(136, 118),    S(155, 75),     S(161, 63),     S(153, 123),    S(147, 78),     S(126, 71),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(406, 241),    S(441, 249),    S(398, 274),    S(405, 258),    S(401, 257),    S(348, 269),    S(363, 271),    S(359, 274),    //
    S(300, 296),    S(364, 285),    S(450, 261),    S(382, 285),    S(384, 287),    S(343, 293),    S(260, 323),    S(257, 322),    //
    S(282, 283),    S(413, 249),    S(440, 243),    S(421, 241),    S(381, 253),    S(332, 276),    S(327, 280),    S(249, 313),    //
    S(258, 273),    S(334, 273),    S(373, 257),    S(351, 253),    S(344, 263),    S(300, 283),    S(280, 284),    S(219, 298),    //
    S(214, 230),    S(285, 240),    S(266, 257),    S(260, 245),    S(251, 252),    S(235, 280),    S(203, 270),    S(178, 269),    //
    S(181, 202),    S(245, 201),    S(248, 223),    S(235, 214),    S(239, 207),    S(209, 245),    S(199, 223),    S(158, 238),    //
    S(93, 218),     S(209, 161),    S(231, 183),    S(239, 185),    S(230, 187),    S(216, 201),    S(193, 185),    S(166, 200),    //
    S(142, 206),    S(159, 228),    S(230, 189),    S(259, 171),    S(238, 190),    S(225, 206),    S(210, 198),    S(190, 221),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(463, 410),    S(543, 344),    S(529, 378),    S(444, 484),    S(458, 447),    S(447, 442),    S(484, 373),    S(385, 449),    //
    S(485, 442),    S(427, 545),    S(432, 554),    S(285, 619),    S(291, 612),    S(359, 569),    S(387, 486),    S(405, 465),    //
    S(421, 499),    S(517, 483),    S(438, 557),    S(406, 573),    S(357, 579),    S(374, 531),    S(443, 438),    S(423, 408),    //
    S(499, 398),    S(493, 467),    S(454, 507),    S(427, 558),    S(417, 560),    S(416, 495),    S(470, 402),    S(465, 370),    //
    S(481, 401),    S(490, 421),    S(464, 469),    S(435, 514),    S(438, 506),    S(434, 468),    S(451, 398),    S(464, 347),    //
    S(467, 319),    S(500, 352),    S(500, 423),    S(465, 403),    S(461, 395),    S(468, 415),    S(474, 356),    S(459, 330),    //
    S(448, 199),    S(489, 156),    S(493, 243),    S(503, 292),    S(485, 320),    S(486, 285),    S(455, 336),    S(456, 321),    //
    S(403, 241),    S(453, 44),     S(455, 69),     S(484, 158),    S(490, 235),    S(492, 185),    S(481, 216),    S(434, 264),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-570, -149),  S(-102, 158),   S(52, 66),      S(-143, 25),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(54, -38),     S(200, 65),     S(164, 64),     S(129, -24),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-18, 93),     S(234, 69),     S(220, 50),     S(112, 5),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-273, 107),   S(149, 43),     S(57, 41),      S(49, 21),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-282, 73),    S(88, 9),       S(88, 1),       S(5, 28),       S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-140, 40),    S(171, -29),    S(112, -12),    S(85, 1),       S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(6, -23),      S(189, -55),    S(128, -36),    S(58, -6),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-178, -13),   S(-9, -19),     S(-111, 1),     S(-111, -22),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(-36, -3), S(8, -17), S(0, -16), S(2, -1), S(6, 1), S(-5, -6), S(3, -6), }},
  {{ S(-5, 0), S(11, -19), S(31, -14), S(25, 13), S(32, 11), S(33, -12), S(14, -17), }},
  {{ S(-7, 1), S(13, -24), S(-2, -0), S(-12, 16), S(-6, 20), S(2, 3), S(8, -20), }},
  {{ S(1, 8), S(15, -1), S(34, -3), S(6, -11), S(4, -6), S(25, 4), S(28, 9), }},
}};
// Epoch duration: 5.44659s
// clang-format on
}  // namespace Clockwork
