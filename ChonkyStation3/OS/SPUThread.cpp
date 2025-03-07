#include "SPUThread.hpp"
#include "PlayStation3.hpp"


SPUThread::SPUThread(PlayStation3* ps3, std::string name) : ps3(ps3) {
    id = ps3->handle_manager.request();
    this->name = name;
    log("Created SPU thread %d \"%s\"\n", id, name.c_str());
}

void SPUThread::loadImage(sys_spu_image* img) {
    log("Importing SPU image for thread %d \"%s\"\n", id, name.c_str());

    // Copy SPU image to LS
    sys_spu_segment* segs = (sys_spu_segment*)ps3->mem.getPtr(img->segs_ptr);
    for (int i = 0; i < img->n_segs; i++) {
        switch (segs[i].type) {

        case SYS_SPU_SEGMENT_TYPE_COPY: {
            log("*  Loading segment %d type COPY\n", i);
            std::memcpy(&ls[segs[i].ls_addr], ps3->mem.getPtr(segs[i].src.addr), segs[i].size);
            log("*  Copied segment from main[0x%08x - 0x%08x] to ls[0x%08x - 0x%08x]\n", (u32)segs[i].src.addr, segs[i].src.addr + segs[i].size, (u32)segs[i].ls_addr, segs[i].ls_addr + segs[i].size);
            break;
        }

        default: {
            Helpers::panic("Unimplemented segment type %d\n", (u32)segs[i].type);
        }
        }
    }

    log("Entry point: 0x%08x\n", (u32)img->entry);
    state.pc = img->entry;

    /*
    std::string filename = std::format("ls{:d}.bin", id);
    std::ofstream file(filename, std::ios::binary);
    file.write((char*)ls, 16_KB);
    file.close();
    */
}

void SPUThread::halt() {
    Helpers::panic("Halting thread %d \"%s\"\n", id, name.c_str());
    status = ThreadStatus::Terminated;
    ps3->spu_thread_manager.reschedule();
}

std::string SPUThread::channelToString(u32 ch) {
    switch (ch) {
    
    case MFC_WrMSSyncReq:       return "MFC_WrMSSyncReq";
    case MFC_RdTagMask:         return "MFC_RdTagMask";
    case MFC_LSA:               return "MFC_LSA";
    case MFC_EAH:               return "MFC_EAH";
    case MFC_EAL:               return "MFC_EAL";
    case MFC_Size:              return "MFC_Size";
    case MFC_TagID:             return "MFC_TagID";
    case MFC_Cmd:               return "MFC_Cmd";
    case MFC_WrTagMask:         return "MFC_WrTagMask";
    case MFC_WrTagUpdate:       return "MFC_WrTagUpdate";
    case MFC_RdTagStat:         return "MFC_RdTagStat";
    case MFC_RdListStallStat:   return "MFC_RdListStallStat";
    case MFC_WrListStallAck:    return "MFC_WrListStallAck";
    case MFC_RdAtomicStat:      return "MFC_RdAtomicStat";

    default: Helpers::panic("Tried to get name of bad MFC channel %d\n", ch);
    }
}

u32 SPUThread::readChannel(u32 ch) {
    log("Read %s\n", channelToString(ch).c_str());

    switch (ch) {
    
    case MFC_RdTagStat:     return 0xffffffff;  break;  // TODO
    case MFC_RdAtomicStat:  return atomic_stat; break;  // TODO

    default:
        Helpers::panic("Unimplemented MFC channel read 0x%02x\n", ch);
    }
}

void SPUThread::writeChannel(u32 ch, u32 val) {
    log("%s = 0x%08x\n", channelToString(ch).c_str(), val);

    switch (ch) {
     
    case MFC_LSA:           lsa     = val;  break;
    case MFC_EAH:           eah     = val;  break;
    case MFC_EAL:           eal     = val;  break;
    case MFC_Size:          size    = val;  break;
    case MFC_TagID:         tag_id  = val;  break;
    case MFC_Cmd:           doCmd(val);     break;
    case MFC_WrTagMask:     /* TODO */      break;
    case MFC_WrTagUpdate:   /* TODO */      break;

    default:
        Helpers::panic("Unimplemented MFC channel write 0x%02x\n", ch);
    }
}

void SPUThread::doCmd(u32 cmd) {
    switch (cmd) {
       
    case GET: {
        log("GET\n");
        std::memcpy(&ls[lsa], ps3->mem.getPtr(eal), size);
        break;
    }

    case PUTLLC: {
        log("PUTLLC\n");

        // Try to acquire the reservation
        bool success = ps3->mem.acquireReservation(eal, id);
        if (success)    // Conditionally write
            std::memcpy(ps3->mem.getPtr(eal), &ls[lsa], size);
        // Update atomic stat
        atomic_stat = 0;
        atomic_stat |= !success;    // Bit 0 is set if the reservation was lost 
        break;
    }

    case GETLLAR: {
        log("GETLLAR\n");

        // Create reservation
        ps3->mem.reserveAddress(eal, id);
        std::memcpy(&ls[lsa], ps3->mem.getPtr(eal), size);
        // Update atomic stat
        atomic_stat = 0;    // It gets overwritten on every command
        atomic_stat |= (1 << 2);    // getllar command completed
        break;
    }

    default:
        Helpers::panic("Unimplemented MFC command 0x%02x\n", cmd);
    }
}