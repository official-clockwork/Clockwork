#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(180, 158);
inline const PParam KNIGHT_MAT = S(549, 327);
inline const PParam BISHOP_MAT = S(517, 281);
inline const PParam ROOK_MAT   = S(572, 420);
inline const PParam QUEEN_MAT  = S(1165, 578);
inline const PParam TEMPO_VAL  = S(65, 13);

inline const PParam BISHOP_PAIR_VAL   = S(82, 165);
inline const PParam ROOK_OPEN_VAL     = S(107, -24);
inline const PParam ROOK_SEMIOPEN_VAL = S(43, 18);

inline const PParam DOUBLED_PAWN_VAL = S(-40, -74);

inline const PParam POTENTIAL_CHECKER_VAL = S(-67, -9);
inline const PParam OUTPOST_KNIGHT_VAL    = S(47, 38);
inline const PParam OUTPOST_BISHOP_VAL    = S(54, 30);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(51, 1);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(57, -26);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(36, 34);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(69, -59);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(25, 15), S(61, 33), S(77, 69), S(189, 146), S(504, 242), S(748, 701),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(73, 38), S(64, 33), S(76, 65), S(178, 115), S(633, -27),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-83, -98), S(-81, -75), S(-58, 6), S(24, 73), S(102, 195), S(316, 279),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(42, -34), S(33, -1), S(26, 24), S(19, 77), S(112, 125), S(219, 223),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(7, -34), S(1, 4), S(-3, -22), S(-5, -41), S(-21, -88), S(-269, -109),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(1, 114), S(-8, 88), S(-2, 32), S(-2, 9), S(11, 9), S(48, 7), S(22, -9),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-280, -11), S(-33, 26), S(-10, 48), S(32, 61), S(38, 83), S(52, 89), S(19, 91),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(-5, -57), S(103, 102), S(159, 160), S(200, 192), S(246, 202), S(275, 236), S(314, 227), S(351, 228), S(400, 166),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-26, -164), S(48, 21), S(117, 80), S(153, 123), S(178, 159), S(199, 178), S(209, 192), S(227, 197), S(242, 206), S(257, 198), S(279, 186), S(338, 142), S(342, 140), S(409, 91),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(254, 94), S(204, 262), S(233, 281), S(251, 286), S(264, 297), S(271, 310), S(278, 318), S(289, 318), S(298, 321), S(307, 325), S(321, 323), S(329, 323), S(339, 322), S(363, 299), S(463, 208),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(476, 166), S(548, 220), S(595, 249), S(621, 361), S(631, 428), S(649, 466), S(653, 510), S(665, 509), S(669, 532), S(676, 540), S(678, 557), S(685, 556), S(699, 540), S(699, 554), S(704, 546), S(710, 539), S(719, 528), S(713, 536), S(719, 517), S(739, 492), S(744, 482), S(778, 432), S(789, 426), S(863, 340), S(935, 281), S(927, 260), S(639, 421), S(810, 308),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(604, -172), S(266, -192), S(132, -87), S(87, -38), S(35, -26), S(-26, -0), S(-75, 37), S(-128, 60), S(-168, 45),
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(261, 225), S(353, 196), S(424, 155),
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(467, 372), S(288, 235), S(159, 72),
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(348, 319), S(462, 315), S(489, 330), S(576, 382), S(700, 351),
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(906, 745), S(616, 611), S(351, 470), S(147, 271), S(77, 8), S(-2, -289),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(-113, 60), S(1, -0), S(-221, -128), S(44, 42), S(-250, -150),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(-26, 22), S(-22, 20), S(-20, 14), S(-15, 9), S(-22, -10),
};

inline const PParam PAWN_THREAT_KNIGHT = S(249, 57);
inline const PParam PAWN_THREAT_BISHOP = S(226, 123);
inline const PParam PAWN_THREAT_ROOK   = S(212, 82);
inline const PParam PAWN_THREAT_QUEEN  = S(190, -39);

inline const PParam KNIGHT_THREAT_BISHOP = S(117, 69);
inline const PParam KNIGHT_THREAT_ROOK   = S(258, 14);
inline const PParam KNIGHT_THREAT_QUEEN  = S(166, -52);

inline const PParam BISHOP_THREAT_KNIGHT = S(121, 36);
inline const PParam BISHOP_THREAT_ROOK   = S(253, 66);
inline const PParam BISHOP_THREAT_QUEEN  = S(198, 57);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(1, -17), S(-4, -5), S(-3, -16), S(-9, -23), S(-15, -29), S(-21, -34), S(-24, -43), S(-28, -47), S(-40, -50),
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(307, 282),    S(248, 346),    S(314, 309),    S(287, 250),    S(339, 184),    S(299, 241),    S(214, 276),    S(290, 246),    //
    S(157, 178),    S(229, 223),    S(209, 168),    S(172, 138),    S(166, 105),    S(133, 141),    S(111, 172),    S(66, 183),     //
    S(106, 140),    S(111, 158),    S(126, 115),    S(113, 107),    S(98, 100),     S(65, 110),     S(31, 144),     S(17, 158),     //
    S(92, 92),      S(107, 122),    S(112, 99),     S(91, 100),     S(63, 94),      S(47, 101),     S(-2, 140),     S(-19, 130),    //
    S(83, 67),      S(147, 74),     S(94, 119),     S(80, 118),     S(46, 116),     S(15, 112),     S(-4, 124),     S(-24, 112),    //
    S(84, 77),      S(211, 83),     S(160, 122),    S(118, 139),    S(82, 128),     S(53, 119),     S(32, 135),     S(-7, 130),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-299, -13),   S(-269, 210),   S(-364, 385),   S(39, 173),     S(-29, 168),    S(-133, 135),   S(-461, 253),   S(-366, 51),    //
    S(102, 136),    S(160, 126),    S(236, 90),     S(236, 121),    S(224, 132),    S(163, 121),    S(146, 131),    S(111, 93),     //
    S(183, 87),     S(216, 131),    S(253, 155),    S(230, 171),    S(196, 174),    S(149, 178),    S(156, 126),    S(93, 135),     //
    S(231, 135),    S(230, 145),    S(237, 173),    S(216, 203),    S(231, 198),    S(202, 181),    S(198, 139),    S(178, 117),    //
    S(230, 120),    S(269, 108),    S(243, 153),    S(235, 169),    S(215, 179),    S(214, 163),    S(205, 126),    S(180, 113),    //
    S(167, 99),     S(200, 87),     S(182, 128),    S(205, 151),    S(207, 148),    S(157, 139),    S(159, 92),     S(121, 85),     //
    S(173, 92),     S(198, 74),     S(182, 86),     S(183, 112),    S(165, 116),    S(141, 78),     S(153, 78),     S(107, 15),     //
    S(120, 62),     S(155, 124),    S(180, 85),     S(194, 82),     S(183, 92),     S(134, 73),     S(122, 101),    S(68, 41),      //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-67, 186),    S(-81, 174),    S(-312, 216),   S(-226, 205),   S(-172, 216),   S(-333, 251),   S(-114, 231),   S(-43, 198),    //
    S(94, 93),      S(54, 158),     S(48, 139),     S(7, 173),      S(-15, 179),    S(38, 165),     S(63, 141),     S(49, 134),     //
    S(133, 121),    S(161, 123),    S(176, 145),    S(124, 159),    S(101, 150),    S(109, 149),    S(142, 131),    S(97, 110),     //
    S(130, 96),     S(137, 126),    S(162, 134),    S(147, 172),    S(174, 165),    S(112, 150),    S(115, 117),    S(83, 119),     //
    S(135, 77),     S(153, 99),     S(158, 127),    S(166, 144),    S(153, 154),    S(116, 149),    S(83, 122),     S(89, 80),      //
    S(144, 85),     S(193, 86),     S(207, 102),    S(159, 138),    S(149, 127),    S(147, 123),    S(157, 102),    S(105, 102),    //
    S(140, 35),     S(218, 48),     S(183, 67),     S(157, 90),     S(138, 99),     S(139, 77),     S(135, 82),     S(131, 39),     //
    S(144, 55),     S(133, 101),    S(143, 108),    S(155, 75),     S(167, 67),     S(160, 112),    S(149, 82),     S(127, 76),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(397, 265),    S(458, 261),    S(380, 297),    S(434, 261),    S(415, 264),    S(353, 289),    S(371, 283),    S(389, 282),    //
    S(299, 316),    S(378, 295),    S(443, 282),    S(391, 303),    S(412, 292),    S(374, 293),    S(291, 326),    S(281, 331),    //
    S(290, 296),    S(413, 262),    S(453, 252),    S(443, 255),    S(418, 254),    S(359, 287),    S(368, 282),    S(270, 330),    //
    S(262, 286),    S(339, 286),    S(383, 273),    S(388, 253),    S(366, 268),    S(320, 292),    S(292, 301),    S(235, 306),    //
    S(219, 238),    S(292, 247),    S(274, 266),    S(273, 257),    S(270, 256),    S(258, 285),    S(223, 283),    S(207, 273),    //
    S(189, 212),    S(250, 210),    S(256, 232),    S(241, 224),    S(255, 210),    S(222, 256),    S(213, 235),    S(174, 244),    //
    S(113, 228),    S(221, 166),    S(247, 186),    S(253, 189),    S(251, 186),    S(234, 204),    S(217, 185),    S(179, 206),    //
    S(155, 217),    S(181, 230),    S(246, 191),    S(273, 178),    S(257, 193),    S(243, 209),    S(224, 207),    S(205, 224),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(498, 411),    S(551, 372),    S(548, 389),    S(443, 519),    S(486, 445),    S(456, 466),    S(495, 378),    S(420, 448),    //
    S(477, 488),    S(451, 555),    S(434, 581),    S(308, 631),    S(330, 604),    S(385, 566),    S(420, 475),    S(436, 473),    //
    S(437, 506),    S(518, 505),    S(438, 580),    S(433, 586),    S(384, 577),    S(388, 562),    S(466, 441),    S(424, 446),    //
    S(499, 425),    S(502, 480),    S(458, 531),    S(453, 569),    S(432, 573),    S(424, 521),    S(482, 423),    S(485, 365),    //
    S(477, 423),    S(492, 435),    S(468, 479),    S(444, 521),    S(446, 512),    S(445, 470),    S(466, 405),    S(475, 362),    //
    S(473, 319),    S(498, 362),    S(501, 434),    S(468, 421),    S(468, 401),    S(478, 418),    S(483, 361),    S(463, 346),    //
    S(452, 219),    S(494, 155),    S(497, 242),    S(512, 296),    S(494, 309),    S(497, 279),    S(463, 344),    S(465, 313),    //
    S(406, 263),    S(465, 59),     S(471, 53),     S(492, 168),    S(502, 233),    S(498, 186),    S(488, 212),    S(437, 274),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-654, -117),  S(-73, 97),     S(-51, 81),     S(-176, 41),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(58, -53),     S(73, 125),     S(154, 83),     S(165, 4),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-39, 84),     S(186, 76),     S(228, 55),     S(177, 16),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-290, 96),    S(160, 29),     S(103, 29),     S(69, 22),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-280, 65),    S(81, 0),       S(107, -2),     S(28, 28),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-167, 40),    S(156, -36),    S(113, -11),    S(84, 8),       S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-9, -26),     S(181, -60),    S(131, -32),    S(63, 3),       S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-209, -13),   S(-23, -21),    S(-105, 6),     S(-123, 4),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

// Epoch duration: 8.9505s
// clang-format on
}  // namespace Clockwork
