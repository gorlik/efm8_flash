/******************************************************************************
 *  EFM8 Flash - Flash tool for Silicon labs EFM8 MCUs                        *
 *  Copyright 2020 Gabriele Gorla                                             *
 *                                                                            *
 *  This program is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  EFM8 Flash is distributed in the hope that it will be useful,             *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with EFM8 Flash.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                            *
 ******************************************************************************/
#ifndef EFM_FLASH_H
#define EFM_FLASH_H

int efm8_find();
int efm8_flash(unsigned char *fw, unsigned int prg_size);

#endif
